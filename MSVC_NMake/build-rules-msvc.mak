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
{..\cairomm\}.cc{$(CFG)\$(PLAT)\cairomm\}.obj::
	$(CXX) $(LIBCAIROMM_CFLAGS) $(CFLAGS_NOGL) /Fo$(CFG)\$(PLAT)\cairomm\ /c @<<
$<
<<

{.\cairomm\}.rc{$(CFG)\$(PLAT)\cairomm\}.res:
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
$(CAIROMM_DLL): $(CFG)\$(PLAT)\cairomm\cairomm.def $(cairomm_OBJS)
	link /DLL $(LDFLAGS_NOLTCG) $(CAIRO_LIB) $(LIBSIGC_LIB) /implib:$(CAIROMM_LIB) /def:$(CFG)\$(PLAT)\cairomm\cairomm.def -out:$@ @<<
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

{.\gendef\}.cc{$(CFG)\$(PLAT)\}.exe:
	@if not exist $(CFG)\$(PLAT)\gendef\ $(MAKE) -f Makefile.vc CFG=$(CFG) $(CFG)\$(PLAT)\gendef
	$(CXX) $(CAIROMM_BASE_CFLAGS) $(CFLAGS) /Fo$(CFG)\$(PLAT)\gendef\ $< /link $(LDFLAGS) /out:$@

{..\examples\text\}.cc{$(CFG)\$(PLAT)\}.exe:
	@if not exist $(CFG)\$(PLAT)\cairomm-ex $(MAKE) -f Makefile.vc CFG=$(CFG) $(CFG)\$(PLAT)\cairomm-ex
	@if not exist $(CAIROMM_LIB) $(MAKE) -f Makefile.vc CFG=$(CFG) $(CAIROMM_LIB)
	$(CXX) $(CAIROMM_EX_CFLAGS) $(CFLAGS) /Fo$(CFG)\$(PLAT)\cairomm-ex\ $< /Fe$@ /link $(LDFLAGS) $(CAIROMM_LIB) $(LIBSIGC_LIB) $(CAIRO_LIB)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

{..\examples\surfaces\}.cc{$(CFG)\$(PLAT)\}.exe:
	@if not exist $(CFG)\$(PLAT)\cairomm-ex $(MAKE) -f Makefile.vc CFG=$(CFG) $(CFG)\$(PLAT)\cairomm-ex
	@if not exist $(CAIROMM_LIB) $(MAKE) -f Makefile.vc CFG=$(CFG) $(CAIROMM_LIB)
	$(CXX) $(CAIROMM_EX_CFLAGS) $(CFLAGS) /Fo$(CFG)\$(PLAT)\cairomm-ex\ $< /Fe$@ /link $(LDFLAGS) $(CAIROMM_LIB) $(LIBSIGC_LIB) $(CAIRO_LIB)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

{..\tests\}.cc{$(CFG)\$(PLAT)\}.exe:
	@if not exist $(CAIROMM_LIB) $(MAKE) -f Makefile.vc CFG=$(CFG) $(CAIROMM_LIB)
	@if not exist $(CFG)\$(PLAT)\cairomm-tests $(MAKE) -f Makefile.vc CFG=$(CFG) $(CFG)\$(PLAT)\cairomm-tests
	$(CXX) $(CAIROMM_EX_CFLAGS) $(CFLAGS) /Fo$(CFG)\$(PLAT)\cairomm-tests\ $< /Fe$@ /link $(LDFLAGS) $(CAIROMM_LIB) $(LIBSIGC_LIB) $(CAIRO_LIB)
	@-if exist $@.manifest mt /manifest $@.manifest /outputresource:$@;1

clean:
	@-del /f /q $(CFG)\$(PLAT)\*.exe
	@-del /f /q $(CFG)\$(PLAT)\*.dll
	@-del /f /q $(CFG)\$(PLAT)\*.pdb
	@-del /f /q $(CFG)\$(PLAT)\*.ilk
	@-del /f /q $(CFG)\$(PLAT)\*.exp
	@-del /f /q $(CFG)\$(PLAT)\*.lib
	@-if exist $(CFG)\$(PLAT)\cairomm-tests del /f /q $(CFG)\$(PLAT)\cairomm-tests\*.obj
	@-del /f /q $(CFG)\$(PLAT)\cairomm-ex\*.obj
	@-del /f /q $(CFG)\$(PLAT)\cairomm\*.res
	@-del /f /q $(CFG)\$(PLAT)\cairomm\*.def
	@-del /f /q $(CFG)\$(PLAT)\cairomm\*.obj
	@-del /f /q $(CFG)\$(PLAT)\gendef\*.obj
	@-if exist $(CFG)\$(PLAT)\cairomm-tests rd $(CFG)\$(PLAT)\cairomm-tests
	@-rd $(CFG)\$(PLAT)\cairomm-ex
	@-rd $(CFG)\$(PLAT)\cairomm
	@-rd $(CFG)\$(PLAT)\gendef
	@-del /f /q vc$(PDBVER)0.pdb
