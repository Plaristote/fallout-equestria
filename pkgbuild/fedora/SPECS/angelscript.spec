Name:           angelscript
Version:        2.28.0
Release:        1%{?dist}
Summary:        Library for cross-platform scripting AngelCode in C++.

License:        zlib
URL:            http://www.angelcode.com
Source0:        http://www.angelcode.com/angelscript/sdk/files/angelscript_2.28.0.zip

BuildRequires:  cmake
#Requires:       

%description
The AngelCode Scripting Library, or AngelScript as it is also known, is an extremely flexible cross-platform scripting library designed to allow applications to extend their functionality through external scripts. It has been designed from the beginning to be an easy to use component, both for the application programmer and the script writer.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%setup -q


%build
cmake angelscript/projects/cmake
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
mkdir %{buildroot}
mkdir %{buildroot}/usr
mkdir %{buildroot}/usr/lib
mkdir %{buildroot}/usr/include
cp angelscript/lib/libAngelscript.a %{buildroot}/usr/lib/libangelscript.a
cp angelscript/include/angelscript.h %{buildroot}/usr/include/angelscript.h

%files
/usr/include/angelscript.h
/usr/lib/libangelscript.a
%doc



%changelog
* Sun Dec 22 2013 Michael Martin Moro <michael.martinmoro@yahoo.fr>
- Initial version of the package