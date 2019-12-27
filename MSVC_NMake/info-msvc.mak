# NMake Makefile portion for displaying config info

all-build-info:
	@echo.
	@echo ----------
	@echo Build info
	@echo ---------
	@echo Build Type: $(CFG)

help:
	@echo.
	@echo ============================
	@echo Building cairomm Using NMake
	@echo ============================
	@echo nmake /f Makefile.vc CFG=[release^|debug] ^<PREFIX=PATH^>
	@echo.
	@echo Where:
	@echo ------
	@echo CFG: Required, use CFG=release for an optimized build and CFG=debug
	@echo for a debug build.  PDB files are generated for all builds.
	@echo.
	@echo PREFIX: Optional, the path where dependent libraries and tools may be
	@echo found, default is ^$(srcrootdir)\..\vs^$(short_vs_ver)\^$(platform),
	@echo where ^$(short_vs_ver) is 14 for VS 2015~2019 and so on; and
	@echo ^$(platform) is Win32 for 32-bit builds and x64 for x64 builds.
	@echo.
	@echo ======
	@echo A 'clean' target is supported to remove all generated files, intermediate
	@echo object files and binaries for the specified configuration.
	@echo.
	@echo An 'install' target is supported to copy the build (DLLs, utility programs,
	@echo LIBs, along with the introspection files if applicable) to appropriate
	@echo locations under ^$(PREFIX).
	@echo.
	@echo A 'tests' target is supported to build the test programs.  Note that the
	@echo Boost libraries are required.  Also pass in BOOST_DLL=1 if the installed
	@echo Boost libraries are built as DLLs.
	@echo ======
	@echo.
