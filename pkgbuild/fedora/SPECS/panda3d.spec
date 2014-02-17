%global __os_install_post    \
    /usr/lib/rpm/redhat/brp-compress \
    %{!?__debug_package:\
    /usr/lib/rpm/redhat/brp-strip %{__strip} \
    /usr/lib/rpm/redhat/brp-strip-comment-note %{__strip} %{__objdump} \
    } \
    /usr/lib/rpm/redhat/brp-strip-static-archive %{__strip} \
    %{!?__jar_repack:/usr/lib/rpm/redhat/brp-java-repack-jars} \
%{nil}

Name:           panda3d
Version:        1.8.1
Release:        1%{?dist}
Summary:        C++ framework assisting the development of 3D applications

License:        BSD
URL:            http://www.panda3d.org
Source0:        http://www.panda3d.org/download/panda3d-1.8.1/panda3d-1.8.1.tar.gz

BuildRequires:  gcc-c++ zlib-devel librocket-devel bison flex libjpeg-devel libtiff-devel libpng-devel openal-devel mesa-libGL-devel
Requires:       zlib librocket libjpeg libtiff libpng openal mesa-libGL

%description

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q


%build
chmod +x makepanda/makepanda.py
makepanda/makepanda.py --everything


%install
rm -rf $RPM_BUILD_ROOT
chmod +x makepanda/installpanda.py
mkdir %{buildroot}
makepanda/installpanda.py --destdir=%{buildroot} --prefix=/usr
# Using the panda3d subdirectory in /usr/lib isn't a good idea:
for file in `find %{buildroot}/usr/lib/panda3d -type f -name "lib*"` ; do
  mv $file %{buildroot}/usr/lib
done


%files
%doc
%config /etc/Confauto.prc
%config /etc/Config.prc
%config /etc/ld.so.conf.d/panda3d.conf
%{_libdir}/*.so
%{_libdir}/*.so.*
%{_libdir}/panda3d
%{_libdir}/python2.7/site-packages/*
/usr/share/panda3d/*
/usr/share/application-registry/panda3d.applications
/usr/share/applications/pview.desktop
/usr/share/mime-info/panda3d.keys
/usr/share/mime-info/panda3d.mime
/usr/share/mime/packages/panda3d.xml
/usr/bin/*

%files devel
%doc
%{_includedir}/*
%{_libdir}/debug/*
/usr/src/debug



%changelog
* Sun Dec 22 2013 Michael Martin Moro <michael.martinmoro@yahoo.fr>
- Initial version of the package