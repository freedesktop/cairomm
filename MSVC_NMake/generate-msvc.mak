# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Create the build directories
vs$(PDBVER)\$(CFG)\$(PLAT)\gendef	\
vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm	\
vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-ex	\
vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm-tests:
	@-mkdir $@

# Generate .def files
vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\cairomm.def: $(GENDEF) vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm $(cairomm_OBJS)
	vs$(PDBVER)\$(CFG)\$(PLAT)\gendef.exe $@ $(CAIROMM_LIBNAME) vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\*.obj
