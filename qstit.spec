Name:           qstit
Version:        3.0
Release:        1%{?dist}
Summary:        Project and manipulate extra subtitles live
License:        GPL-3.0-or-later
URL:            https://github.com/adamos2468/qstit
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  qt6-qtbase-devel
BuildRequires:  desktop-file-utils

%description
Qstit is a surtitling program: it projects subtitles line by line on a
second screen during a live screening or performance, either by hand or
timed from an SRT file, and lets the operator correct them on the fly.

%prep
%autosetup

%build
cd src
%qmake_qt6 PREFIX=%{_prefix}
# -f Makefile: bypass the bootstrap GNUmakefile, which would run qmake again
# without the Fedora build flags
%make_build -f Makefile

%install
cd src
%make_install -f Makefile INSTALL_ROOT=%{buildroot}

%check
desktop-file-validate %{buildroot}%{_datadir}/applications/%{name}.desktop

%files
%license COPYING
%doc README.md
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/512x512/apps/%{name}.png
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%changelog
* Sun Nov 22 2026 Adamos Ttofari <adamos2468@gmail.com> - 3.0-1
- First Fedora package, built against Qt6
