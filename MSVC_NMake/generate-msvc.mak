# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Generate .def files
vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\cairomm.def: $(GENDEF) $(cairomm_OBJS)
	vs$(PDBVER)\$(CFG)\$(PLAT)\gendef.exe $@ $(CAIROMM_LIBNAME) vs$(PDBVER)\$(CFG)\$(PLAT)\cairomm\*.obj
