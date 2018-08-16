# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Create the build directories
$(CFG)\$(PLAT)\gendef $(CFG)\$(PLAT)\cairomm $(CFG)\$(PLAT)\cairomm-ex $(CFG)\$(PLAT)\cairomm-tests:
	@-mkdir $@

# Generate .def files
$(CFG)\$(PLAT)\cairomm\cairomm.def: $(GENDEF) $(CFG)\$(PLAT)\cairomm $(cairomm_OBJS)
	$(CFG)\$(PLAT)\gendef.exe $@ $(CAIROMM_LIBNAME) $(CFG)\$(PLAT)\cairomm\*.obj
