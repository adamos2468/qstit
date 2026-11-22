#!/bin/bash
# Start Qstit without a display and check that it comes up cleanly. Two runs:
#
#  1. Under gdb: it must not crash, not quit by itself, and not show a
#     message box while starting (at start-up a message box only ever means
#     something went wrong, such as the language file failing to load).
#  2. Under valgrind: it must not read a value that was never set, or memory
#     it does not own. Such a bug can depend on whatever happens to be left
#     in memory, so it may crash on one computer and not another; valgrind
#     reports it every time.
#
# Usage: smoke-test.sh [binary]
# Needs gdb and valgrind. Run it on a packaged build: gdb needs the symbol
# names to recognise the message box, and with the -debuginfo package
# installed the valgrind reports show the source lines.

set -u
BIN=${1:-/usr/bin/qstit}
GDB_WAIT=10
VALGRIND_WAIT=30
LOG=$(mktemp)
trap 'rm -f "$LOG"' EXIT

# Qt draws into memory instead of a screen, and Qstit keeps its
# configuration in a throwaway home folder
export QT_QPA_PLATFORM=offscreen
HOME=$(mktemp -d)
export HOME

fail() {
    echo "::error::$1"
    echo "--- $2 output:"
    cat "$LOG"
    exit 1
}

# --- 1. gdb ------------------------------------------------------------------
# After the wait, SIGINT makes gdb pause the program and run the remaining
# commands; if it is still waiting by then, it gets killed
timeout --kill-after=30 -s INT "$GDB_WAIT" \
    gdb -q -batch -nx \
        -ex "set debuginfod enabled off" \
        -ex "set disable-randomization off" \
        -ex "set breakpoint pending off" \
        -ex "break messSkin::fMess" \
        -ex "run" \
        -ex "bt 10" \
        -ex "kill" \
        "$BIN" > "$LOG" 2>&1

grep -q "^Breakpoint 1 at" "$LOG" \
    || fail "could not watch for message boxes: messSkin::fMess not found in $BIN" gdb
grep -qE "(^| hit )Breakpoint 1, " "$LOG" \
    && fail "Qstit showed a message box while starting up; the backtrace below shows where" gdb
grep -qE "received signal SIG(SEGV|ABRT|BUS|FPE|ILL)" "$LOG" \
    && fail "Qstit crashed while starting up" gdb
grep -qE "^\[Inferior 1 .* exited" "$LOG" \
    && fail "Qstit quit by itself while starting up" gdb
grep -q "received signal SIGINT" "$LOG" \
    || fail "Qstit did not start as expected" gdb
echo "gdb: Qstit started, showed no message and was still running after $GDB_WAIT seconds"

# --- 2. valgrind -------------------------------------------------------------
# valgrind is stopped by the timeout, so its exit code says nothing: count
# the reports instead (each one starts with a line like "==123== Invalid read")
timeout --kill-after=30 -s INT "$VALGRIND_WAIT" \
    valgrind -q --num-callers=20 --track-origins=yes "$BIN" > "$LOG" 2>&1

REPORTS=$(grep -E "^==[0-9]+== [A-Z]" "$LOG" | grep -vc "signal 2 (SIGINT)")
[ "$REPORTS" -eq 0 ] \
    || fail "valgrind found $REPORTS memory problem(s) while Qstit was starting up; see the report below" valgrind
echo "valgrind: no memory problems in $VALGRIND_WAIT seconds"
