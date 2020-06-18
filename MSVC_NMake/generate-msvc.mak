# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Generate .def files
vs$(VSVER)\$(CFG)\$(PLAT)\cairomm\cairomm.def: $(GENDEF) $(cairomm_OBJS)
	$(GENDEF) $@ $(CAIROMM_LIBNAME) $(@D)\*.obj

# Generate pre-generated resources and configuration headers (builds from GIT)
prep-git-build: pkg-ver.mak
	$(MAKE) /f Makefile.vc CFG=$(CFG) GENERATE_VERSIONED_FILES=1 cairomm\cairomm.rc cairomm\cairommconfig.h

cairomm\cairomm.rc: pkg-ver.mak cairomm\cairomm.rc.in
	@echo Generating $@...
	@copy $@.in $@
	@$(PERL) -pi.bak -e "s/\@CAIROMM_MAJOR_VERSION\@/$(PKG_MAJOR_VERSION)/g" $@
	@$(PERL) -pi.bak -e "s/\@CAIROMM_MINOR_VERSION\@/$(PKG_MINOR_VERSION)/g" $@
	@$(PERL) -pi.bak -e "s/\@CAIROMM_MICRO_VERSION\@/$(PKG_MICRO_VERSION)/g" $@
	@$(PERL) -pi.bak -e "s/\@VERSION\@/$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION).$(PKG_MICRO_VERSION)/g" $@
	@del $@.bak

# You may change CAIROMM_EXCEPTIONS_ENABLED if you know what you are doing
cairomm\cairommconfig.h: pkg-ver.mak ..\cairommconfig.h.in
	@echo Generating $@...
	@copy ..\$(@F).in $@
	@$(PERL) -pi.bak -e "s/\#undef CAIROMM_EXCEPTIONS_ENABLED/\#define CAIROMM_EXCEPTIONS_ENABLED 1/g" $@
	@$(PERL) -pi.bak -e "s/\#undef CAIROMM_MAJOR_VERSION/\#define CAIROMM_MAJOR_VERSION $(PKG_MAJOR_VERSION)/g" $@
	@$(PERL) -pi.bak -e "s/\#undef CAIROMM_MINOR_VERSION/\#define CAIROMM_MINOR_VERSION $(PKG_MINOR_VERSION)/g" $@
	@$(PERL) -pi.bak -e "s/\#undef CAIROMM_MICRO_VERSION/\#define CAIROMM_MICRO_VERSION $(PKG_MICRO_VERSION)/g" $@
	@del $@.bak

pkg-ver.mak: ..\configure.ac
	@echo Generating version info Makefile Snippet...
	@$(PERL) -00 -ne "print if /AC_INIT\(/" $** |	\
	$(PERL) -pe "tr/, /\n/s" |	\
	$(PERL) -ne "print if 2 .. 2" |	\
	$(PERL) -ne "print /\[(.*)\]/" > ver.txt
	@echo @echo off>pkg-ver.bat
	@echo.>>pkg-ver.bat
	@echo set /p cairomm_ver=^<ver.txt>>pkg-ver.bat
	@echo for /f "tokens=1,2,3 delims=." %%%%a IN ("%cairomm_ver%") do (echo PKG_MAJOR_VERSION=%%%%a^& echo PKG_MINOR_VERSION=%%%%b^& echo PKG_MICRO_VERSION=%%%%c)^>$@>>pkg-ver.bat
	@pkg-ver.bat
	@del ver.txt pkg-ver.bat
