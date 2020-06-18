# Convert the source listing to object (.obj) listing in
# another NMake Makefile module, include it, and clean it up.
# This is a "fact-of-life" regarding NMake Makefiles...
# This file does not need to be changed unless one is maintaining the NMake Makefiles

# For those wanting to add things here:
# To add a list, do the following:
# # $(description_of_list)
# if [call create-lists.bat header $(makefile_snippet_file) $(variable_name)]
# endif
#
# if [call create-lists.bat file $(makefile_snippet_file) $(file_name)]
# endif
#
# if [call create-lists.bat footer $(makefile_snippet_file)]
# endif
# ... (repeat the if [call ...] lines in the above order if needed)
# !include $(makefile_snippet_file)
#
# (add the following after checking the entries in $(makefile_snippet_file) is correct)
# (the batch script appends to $(makefile_snippet_file), you will need to clear the file unless the following line is added)
#!if [del /f /q $(makefile_snippet_file)]
#!endif

# In order to obtain the .obj filename that is needed for NMake Makefiles to build DLLs/static LIBs or EXEs, do the following
# instead when doing 'if [call create-lists.bat file $(makefile_snippet_file) $(file_name)]'
# (repeat if there are multiple $(srcext)'s in $(source_list), ignore any headers):
# !if [for %c in ($(source_list)) do @if "%~xc" == ".$(srcext)" @call create-lists.bat file $(makefile_snippet_file) $(intdir)\%~nc.obj]
#
# $(intdir)\%~nc.obj needs to correspond to the rules added in build-rules-msvc.mak
# %~xc gives the file extension of a given file, %c in this case, so if %c is a.cc, %~xc means .cc
# %~nc gives the file name of a given file without extension, %c in this case, so if %c is a.cc, %~nc means a

NULL=

# For cairomm

!if [call create-lists.bat header cairomm.mak cairomm_OBJS]
!endif

!if [for %c in ($(cairomm_cc)) do @if "%~xc" == ".cc" @call create-lists.bat file cairomm.mak vs^$(VSVER)\^$(CFG)\^$(PLAT)\cairomm\%~nc.obj]
!endif

!if [@call create-lists.bat file cairomm.mak vs^$(VSVER)\^$(CFG)\^$(PLAT)\cairomm\cairomm.res]
!endif

!if [call create-lists.bat footer cairomm.mak]
!endif

!if [call create-lists.bat header cairomm.mak cairomm_ex]
!endif

!if [for %c in (..\examples\text\*.cc) do @call create-lists.bat file cairomm.mak vs^$(VSVER)\^$(CFG)\^$(PLAT)\%~nc.exe]
!endif

!if [for %c in (..\examples\surfaces\*.cc) do @call create-lists.bat file cairomm.mak vs^$(VSVER)\^$(CFG)\^$(PLAT)\%~nc.exe]
!endif

!if [call create-lists.bat footer cairomm.mak]
!endif

!if [call create-lists.bat header cairomm.mak cairomm_tests]
!endif

!if [for %c in (..\tests\*.cc) do @call create-lists.bat file cairomm.mak vs^$(VSVER)\^$(CFG)\^$(PLAT)\%~nc.exe]
!endif

!if [call create-lists.bat footer cairomm.mak]
!endif

!include cairomm.mak

!if [del /f /q cairomm.mak]
!endif
