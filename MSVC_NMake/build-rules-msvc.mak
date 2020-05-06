# NMake Makefile portion for compilation rules
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.  The format
# of NMake Makefiles here are different from the GNU
# Makefiles.  Please see the comments about these formats.

# Inference rules for compiling the .obj files.
# Used for libs and programs with more than a single source file.
# Format is as follows
# (all dirs must have a trailing '\'):
#
# {$(srcdir)}.$(srcext){$(destdir)}.obj::
# 	$(CC)|$(CXX) $(cflags) /Fo$(destdir) /c @<<
# $<
# <<
{..\cairomm\}.cc{vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\}.obj::
	@if not exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\ md vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm
	$(CXX) $(LIBCAIROMM_CFLAGS) $(CFLAGS_NOGL) /Fovs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\ /Fdvs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\ /c @<<
$<
<<

{.\cairomm\}.rc{vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\}.res:
	@if not exist $(@D)\ md $(@D)
	rc /fo$@ $<

# Rules for building .lib files
$(CAIROMM_LIB): $(CAIROMM_DLL)

# Rules for linking DLLs
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link /DLL [$(linker_flags)] [$(dependent_libs)] [/def:$(def_file_if_used)] [/implib:$(lib_name_if_needed)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2
$(CAIROMM_DLL): $(cairomm_OBJS) $(EXTRA_CAIROMM_DEPENDENCIES)
	link /DLL $(LDFLAGS_NOLTCG) $(CAIRO_LIB) $(LIBSIGC_LIB) /implib:$(CAIROMM_LIB) $(EXTRA_CAIROMM_LDFLAG) -out:$@ @<<
$(cairomm_OBJS)
<<
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;2

# Rules for linking Executables
# Format is as follows (the mt command is needed for MSVC 2005/2008 builds):
# $(dll_name_with_path): $(dependent_libs_files_objects_and_items)
#	link [$(linker_flags)] [$(dependent_libs)] -out:$@ @<<
# $(dependent_objects)
# <<
# 	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

{..\MSVC_NMake\gendef\}.cc{vs$(PDBVER)\$(CFG)\$(PLAT)\}.exe:
	@if not exist $(@D)\gendef\ md $(@D)\gendef
	$(CXX) $(CFLAGS) /Fo$(@D)\gendef\ /Fd$(@D)\gendef\ $< /Fe$@ /link $(LDFLAGS)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

{..\examples\text\}.cc{vs$(PDBVER)\$(CFG)\$(PLAT)\}.exe:
	@if not exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex\ md vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex
	@if not exist $(CAIROMM_LIB) $(MAKE) -f Makefile.vc CFG=$(CFG) $(CAIROMM_LIB)
	$(CXX) $(CAIROMM_EX_CFLAGS) $(CFLAGS) /Fo$(@D)\cairomm-ex\ /Fd$(@D)\cairomm-ex\ $< /Fe$@ /link $(LDFLAGS) $(CAIROMM_LIB) $(LIBSIGC_LIB) $(CAIRO_LIB)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

{..\examples\surfaces\}.cc{vs$(PDBVER)\$(CFG)\$(PLAT)\}.exe:
	@if not exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex\ md vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex
	@if not exist $(CAIROMM_LIB) $(MAKE) -f Makefile.vc CFG=$(CFG) $(CAIROMM_LIB)
	$(CXX) $(CAIROMM_EX_CFLAGS) $(CFLAGS) /Fo$(@D)\cairomm-ex\ /Fd$(@D)\cairomm-ex\ $< /Fe$@ /link $(LDFLAGS) $(CAIROMM_LIB) $(LIBSIGC_LIB) $(CAIRO_LIB)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

{..\tests\}.cc{vs$(PDBVER)\$(CFG)\$(PLAT)\}.exe:
	@if not exist $(CAIROMM_LIB) $(MAKE) -f Makefile.vc CFG=$(CFG) $(CAIROMM_LIB)
	@if not exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests\ md vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests
	$(CXX) $(CAIROMM_TEST_CFLAGS) $(CFLAGS) /Fo$(@D)\cairomm-tests\ /Fd$(@D)\cairomm-tests\ $< /Fe$@ /link $(LDFLAGS) $(CAIROMM_LIB) $(LIBSIGC_LIB) $(CAIRO_LIB)

clean:
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.exe
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.dll
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.pdb
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.ilk
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.exp
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\*.lib
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests\*.obj
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests\*.pdb
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex\*.obj
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex\*.pdb
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\*.res
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\*.def
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\*.obj
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\*.pdb
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\gendef\*.obj
	@-del /f /q vs$(PDBVER)\$(CFG)\$(PLAT)\gendef\*.pdb
	@-if exist vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests rd vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests
	@-rd vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex
	@-rd vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm
	@-rd vs$(PDBVER)\$(CFG)\$(PLAT)\gendef
