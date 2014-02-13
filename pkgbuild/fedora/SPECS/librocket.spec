Name:           librocket
Version:        master
Release:        1%{?dist}
Summary:        C++ user interface packaged based on the HTML and CSS standards

License:        MIT
URL:            https://github.com/lloydw/libRocket
Source0:        https://github.com/lloydw/libRocket/archive/master.zip

BuildRequires:  gcc-c++ cmake freetype-devel zlib-devel boost-python-devel
Requires:       freetype zlib boost-python

%description
libRocket is the C++ user interface package based on the HTML and CSS standards. It is designed as a complete solution for any project's interface needs.

libRocket uses the time-tested open standards XHTML1.0 and CSS2.0 (while borrowing features from HTML5 and CSS3), and extends them with features suited towards real-time applications. Because of this, you don't have to learn a whole new proprietary technology like other libraries in this space.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%setup -q


%build
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr Build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
%make_install
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%doc
%{_libdir}/*.so.*

%files devel
%doc
%{_includedir}/*
%{_libdir}/*.so


%changelog
* Sat Dec 21 2013 Michael Martin Moro <michael.martinmoro@yahoo.fr>
- Initial version of the package