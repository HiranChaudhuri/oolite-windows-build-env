# -*- coding: utf-8 -*-
%define nasm_version 2.10.09
Summary: The Netwide Assembler, a portable x86 assembler with Intel-like syntax
Name: nasm
Version: 2.10.09
Release: 1
License: BSD
Group: Development/Languages
Source: http://www.nasm.us/pub/nasm/releasebuilds/%{nasm_version}/nasm-%{nasm_version}.tar.xz
URL: http://www.nasm.us/
BuildRoot: /tmp/rpm-build-nasm
Prefix: %{_prefix}
BuildRequires: perl
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%package doc
Summary: Extensive documentation for NASM
Group: Development/Languages
BuildRequires: ghostscript, texinfo
Requires(post): /sbin/install-info
Requires(preun): /sbin/install-info

%package rdoff
Summary: Tools for the RDOFF binary format, sometimes used with NASM.
Group: Development/Tools

%description
NASM is the Netwide Assembler, a free portable assembler for the Intel
80x86 microprocessor series, using primarily the traditional Intel
instruction mnemonics and syntax.

%description doc
Extensive documentation for the Netwide Assembler, NASM, in HTML,
info, PostScript and text formats.

%description rdoff
Tools for the operating-system independent RDOFF binary format, which
is sometimes used with the Netwide Assembler (NASM).  These tools
include linker, library manager, loader, and information dump.

%prep
%setup -q -n nasm-%{nasm_version}

%build
%configure
make %{?_smp_mflags} everything

%install
rm -rf "$RPM_BUILD_ROOT"
mkdir -p "$RPM_BUILD_ROOT"/%{_bindir}
mkdir -p "$RPM_BUILD_ROOT"/%{_mandir}/man1
mkdir -p "$RPM_BUILD_ROOT"/%{_infodir}
mkdir -p "$RPM_BUILD_ROOT"/usr/tempdoc
%makeinstall install_everything	docdir="$RPM_BUILD_ROOT"/usr/tempdoc
cp -a $RPM_BUILD_ROOT/usr/tempdoc/html \
   $RPM_BUILD_ROOT/usr/tempdoc/nasmdoc.{ps,txt,pdf} .
gzip -9f nasmdoc.{ps,txt}
# remove unpackaged files from the buildroot
rm -rf $RPM_BUILD_ROOT%{_prefix}/tempdoc

%clean
rm -rf "$RPM_BUILD_ROOT"

%post
/sbin/install-info %{_infodir}/nasm.info.gz %{_infodir}/dir || :

%preun
if [ $1 = 0 ]; then
  /sbin/install-info --delete %{_infodir}/nasm.info.gz %{_infodir}/dir || :
fi

%files
%defattr(-,root,root)
%doc AUTHORS CHANGES README TODO
%{_bindir}/nasm
%{_bindir}/ndisasm
%{_mandir}/man1/nasm.1*
%{_mandir}/man1/ndisasm.1*
%{_infodir}/nasm.info*.gz

%files doc
%defattr(-,root,root)
%doc html nasmdoc.txt.gz nasmdoc.pdf nasmdoc.ps.gz

%files rdoff
%defattr(-,root,root)
%{_bindir}/ldrdf
%{_bindir}/rdf2bin
%{_bindir}/rdf2com
%{_bindir}/rdf2ihx
%{_bindir}/rdf2ith
%{_bindir}/rdf2srec
%{_bindir}/rdfdump
%{_bindir}/rdflib
%{_bindir}/rdx
%{_mandir}/man1/ldrdf.1*
%{_mandir}/man1/rdf2bin.1*
%{_mandir}/man1/rdf2com.1*
%{_mandir}/man1/rdf2ihx.1*
%{_mandir}/man1/rdf2ith.1*
%{_mandir}/man1/rdf2srec.1*
%{_mandir}/man1/rdfdump.1*
%{_mandir}/man1/rdflib.1*
%{_mandir}/man1/rdx.1*
