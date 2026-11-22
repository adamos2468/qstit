Name:           qstit
Version:        3.0.1
Release:        1%{?dist}
Summary:        Project and manipulate extra subtitles live
License:        GPL-3.0-or-later
URL:            https://github.com/adamos2468/qstit
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  qt6-qtbase-devel
BuildRequires:  desktop-file-utils
BuildRequires:  libappstream-glib

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
appstream-util validate-relax --nonet %{buildroot}%{_metainfodir}/*.metainfo.xml

%files
%license COPYING
%doc README.md
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_metainfodir}/io.github.adamos2468.qstit.metainfo.xml
%{_datadir}/icons/hicolor/512x512/apps/%{name}.png
%{_datadir}/icons/hicolor/scalable/apps/%{name}.svg

%changelog
* Sun Nov 22 2026 Adamos Ttofari <adamos2468@gmail.com> - 3.0.1-1
- Fix the crash at start-up, which kept Qstit from launching
- Start the text rotation at 0 and the jump step at 0.2 s, instead of
  whatever was left in memory

* Sun Nov 22 2026 Adamos Ttofari <adamos2468@gmail.com> - 3.0-2
- Add AppStream metadata, so that GNOME Software shows the name and icon

* Sun Nov 22 2026 Adamos Ttofari <adamos2468@gmail.com> - 3.0-1
- First Fedora package, built against Qt6
