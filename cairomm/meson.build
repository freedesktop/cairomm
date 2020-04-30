# cairomm

# Input: cairomm_build_dep, cairomm_pcname, cairomm_libversion, cairomm_rc,
#        is_msvc, gendef
# Output: source_h_files, cairomm_dep

# There are no built source files in cairomm.

cairomm_cc = [
  'context.cc',
  'context_surface_quartz.cc',
  'context_surface_win32.cc',
  'context_surface_xlib.cc',
  'device.cc',
  'exception.cc',
  'fontface.cc',
  'fontoptions.cc',
  'matrix.cc',
  'path.cc',
  'pattern.cc',
  'private.cc',
  'quartz_font.cc',
  'quartz_surface.cc',
  'region.cc',
  'scaledfont.cc',
  'script.cc',
  'script_surface.cc',
  'surface.cc',
  'win32_font.cc',
  'win32_surface.cc',
  'xlib_surface.cc',
]

cairomm_public_h = [
  'cairomm.h',
  'context.h',
  'device.h',
  'enums.h',
  'exception.h',
  'fontface.h',
  'fontoptions.h',
  'matrix.h',
  'path.h',
  'pattern.h',
  'quartz_font.h',
  'quartz_surface.h',
  'refptr.h',
  'region.h',
  'scaledfont.h',
  'script.h',
  'script_surface.h',
  'surface.h',
  'types.h',
  'win32_font.h',
  'win32_surface.h',
  'xlib_surface.h',
]

cairomm_private_h = [
  'context_private.h',
  'private.h',
]

# docs/reference/meson.build needs this.
source_h_files = cairomm_public_h

install_headers(cairomm_public_h, subdir: cairomm_pcname / 'cairomm')

# Make sure we are exporting the symbols from the DLL
cairomm_cpp_args = ['-DCAIROMM_BUILD=1']

# Build the .rc file for Windows builds and link to it
extra_cairomm_objects = []
if host_machine.system() == 'windows'
  windows = import('windows')
  cairomm_res = windows.compile_resources(cairomm_rc)
  extra_cairomm_objects += cairomm_res
endif

extra_include_dirs = ['..']

if not is_msvc
  cairomm_library = library(cairomm_pcname,
    cairomm_cc, extra_cairomm_objects,
    version: cairomm_libversion,
    include_directories: extra_include_dirs,
    cpp_args: cairomm_cpp_args,
    dependencies: cairomm_build_dep,
    install: true,
  )
else # is_msvc

  # Run gendef.exe to get the .def file needed for obtaining
  # the .lib file for the cairomm DLL.
  cairomm_int_lib = static_library('cairomm-int',
    cairomm_cc,
    include_directories: extra_include_dirs,
    cpp_args: cairomm_cpp_args,
    dependencies: cairomm_build_dep,
    install: false,
  )

  cairomm_def = custom_target('cairomm.def',
    output: 'cairomm.def',
    depends: cairomm_int_lib,
    command: [ gendef,
      '@OUTPUT@',
      '@0@-@1@.dll'.format(cairomm_pcname, cairomm_libversion.split('.')[0]),
      cairomm_int_lib.full_path(),
    ],
    install: false,
  )
  cairomm_extra_link_args = ['/def:@0@'.format(cairomm_def.full_path())]

  cairomm_library = library(cairomm_pcname, extra_cairomm_objects,
    objects: cairomm_int_lib.extract_all_objects(),
    version: cairomm_libversion,
    dependencies: cairomm_build_dep,
    link_depends: cairomm_def,
    link_args: cairomm_extra_link_args,
    install: true,
  )
endif

# This is useful in the main project when cairomm is used as a subproject.
# It's also used when building example programs and test programs.
cairomm_dep = declare_dependency(
  link_with: cairomm_library,
  include_directories: extra_include_dirs,
  dependencies: cairomm_build_dep
)
