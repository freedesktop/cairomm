# This file is part of cairomm.

project('cairomm', 'cpp',
  version: '1.14.2',
  license: 'LGPLv2+',
  default_options: [
    'cpp_std=c++11',
  ],
  meson_version: '>= 0.54.0', # required for meson.override_dependency()
                              # and dep.get_variable(internal:)
)

cairomm_api_version = '1.0'
cairomm_pcname = meson.project_name() + '-' + cairomm_api_version

cairomm_version_array = meson.project_version().split('.')
cairomm_major_version = cairomm_version_array[0].to_int()
cairomm_minor_version = cairomm_version_array[1].to_int()
cairomm_micro_version = cairomm_version_array[2].to_int()

# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
# The relation between libtool's current:revison:age interface versioning
# and the .so filename, .so.x.y.z, is
# x = current - age
# y = age
# z = revision
# If libtool_soversion is updated as described in libtool's documentation,
# x.y.z will usually *not* be equal to meson.project_version().
libtool_soversion = [5, 0, 4]
cairomm_libversion = '@0@.@1@.@2@'.format(
  libtool_soversion[0] - libtool_soversion[2],
  libtool_soversion[2],
  libtool_soversion[1]
)
macos_darwin_versions = [
  libtool_soversion[0] + 1,
  '@0@.@1@'.format(libtool_soversion[0] + 1, libtool_soversion[1])
]

# Use these instead of meson.source_root() and meson.build_root() in subdirectories.
# source_root() and build_root() are not useful, if this is a subproject.
project_source_root = meson.current_source_dir()
project_build_root = meson.current_build_dir()

cpp_compiler = meson.get_compiler('cpp')
is_msvc = cpp_compiler.get_id() == 'msvc'
python3 = import('python').find_installation()

python_version = python3.language_version()
python_version_req = '>= 3.5'
if not python_version.version_compare(python_version_req)
  error('Requires Python @0@, found @1@.'.format(python_version_req, python_version))
endif

# Do we build from a git repository?
# Suppose we do if and only if a '.git' directory or file exists.
cmd_py = '''
import os
import sys
sys.exit(os.path.isdir("@0@") or os.path.isfile("@0@"))
'''.format(project_source_root / '.git')
is_git_build = run_command(python3, '-c', cmd_py).returncode() != 0

# Are we testing a dist tarball while it's being built?
# There ought to be a better way. https://github.com/mesonbuild/meson/issues/6866
is_dist_check = project_source_root.contains('dist-unpack') and \
                project_build_root.contains('dist-build')

# Options.
maintainer_mode_opt = get_option('maintainer-mode')
maintainer_mode = maintainer_mode_opt == 'true' or \
                 (maintainer_mode_opt == 'if-git-build' and is_git_build)
if is_dist_check
  message('Looks like a tarball is being tested. ' + \
          'Option "dist-warnings" is used instead of "warnings".')
  warning_level = get_option('dist-warnings')
else
  warning_level = get_option('warnings')
endif
build_deprecated_api = get_option('build-deprecated-api')
build_exceptions_api = get_option('build-exceptions-api')
build_documentation_opt = get_option('build-documentation')
build_documentation = build_documentation_opt == 'true' or \
                     (build_documentation_opt == 'if-maintainer-mode' and maintainer_mode)
build_examples = get_option('build-examples')
build_tests_opt = get_option('build-tests')

use_msvc14x_toolset_ver = get_option('msvc14x-parallel-installable')

# Installation directories are relative to {prefix}.
install_prefix = get_option('prefix')
install_includedir = get_option('includedir')
install_libdir = get_option('libdir')
install_datadir = get_option('datadir')
install_pkgconfigdir = install_libdir / 'pkgconfig'

# Dependencies.
# cairomm_build_dep: Dependencies when building the cairomm library.
# cairomm_dep (created in cairomm/meson.build):
#   Dependencies when using the cairomm library.
sigcxx_req = '>= 2.6.0'
cairo_req = '>= 1.12.0'

# There are pkg-config files for sigc++ on MSVC, so just use that.
sigcxx_dep = dependency('sigc++-2.0', version: sigcxx_req)
cairo_dep = dependency('cairo', version: cairo_req, required: not is_msvc)

# This is clearly on Windows...
if not cairo_dep.found()
  cairo_dep = cpp_compiler.find_library('cairo', has_headers: [ 'cairo.h', 'cairo-win32.h' ])
endif

cairomm_build_dep = [sigcxx_dep, cairo_dep]
cairomm_requires = [
  'sigc++-2.0', sigcxx_req,
]

cairomm_extra_libs = ''
if cairo_dep.type_name() == 'pkgconfig'
  cairomm_requires += [ 'cairo', cairo_req ]
else
  cairomm_extra_libs = '-lcairo'
endif
cairomm_requires = ' '.join(cairomm_requires)

# Needed for M_PI on Visual Studio and some MinGW versions
if host_machine.system() == 'windows'
  add_project_arguments('-D_USE_MATH_DEFINES', language: 'cpp')
endif

# Some dependencies are required only in maintainer mode and/or
# if documentation shall be built.
mm_common_get = find_program('mm-common-get', required: false)

if maintainer_mode and not mm_common_get.found()
  error('Maintainer mode requires the \'mm-common-get\' command.\n' +
        'Use \'-Dmaintainer-mode=false\' or install the \'mm-common\' package, version 1.0.0 or higher')
endif

perl = find_program('perl', required: build_documentation)
doxygen = find_program('doxygen', required: build_documentation)
dot = find_program('dot', required: build_documentation) # Used by Doxygen
xsltproc = find_program('xsltproc', required: build_documentation)

# Some dependencies are required only for building the test programs.
USE_SHARED_BOOST = get_option('boost-shared') # Use shared Boost::Test?

boost_unit_test_framework_dep = dependency('boost', modules: 'unit_test_framework',
  static: not USE_SHARED_BOOST, version: '>=1.63.0', required: false)

fontconfig_dep = dependency('fontconfig', required: false)
test_dep = [ boost_unit_test_framework_dep, fontconfig_dep]
can_test = boost_unit_test_framework_dep.found() and \
           (fontconfig_dep.found() or host_machine.system() == 'windows')
build_tests = build_tests_opt == 'true' or \
             (build_tests_opt == 'if-dependencies-found' and can_test)
if build_tests and not can_test
  error('Building the test programs requires Boost Test, version 1.33.1 or higher and Fontconfig.\n' + \
        'Install them or use -Dbuild-tests=false or -Dbuild-tests=if-dependencies-found')
endif

script_dir = project_source_root / 'untracked' / 'build_scripts'
doc_reference_py = script_dir / 'doc-reference.py'
dist_changelog_py = script_dir / 'dist-changelog.py'
dist_build_scripts_py = script_dir / 'dist-build-scripts.py'

if maintainer_mode
  # Copy files to untracked/build_scripts and untracked/docs/docs.
  run_command(mm_common_get, '--force', script_dir,
    project_source_root / 'untracked' / 'docs')
else
  cmd_py = '''
import os
import sys
sys.exit(os.path.isfile("@0@"))
'''.format(doc_reference_py)
  file_exists = run_command(python3, '-c', cmd_py).returncode() != 0
  if not file_exists
    warning('Missing files in untracked/. ' + \
    'Enable maintainer-mode if you want to build documentation or create a dist tarball.')
  endif
endif

# Set compiler warnings.
warning_flags = []
if warning_level == 'min'
  if is_msvc
    warning_flags = ['/W3']
  else
    warning_flags = ['-Wall']
  endif
elif warning_level == 'max' or warning_level == 'fatal'
  if is_msvc
    warning_flags = ['/W4']
  else
    warning_flags = '-pedantic -Wall -Wextra -Wformat-security -Wsuggest-override -Wzero-as-null-pointer-constant'.split()
  endif
  if warning_level == 'fatal'
    if is_msvc
      warning_flags += ['/WX']
    else
      warning_flags += ['-Werror']
    endif
    warning_flags += ['-DSIGCXX_DISABLE_DEPRECATED']
  endif
endif

warning_flags = cpp_compiler.get_supported_arguments(warning_flags)
add_project_arguments(warning_flags, language: 'cpp')

# Add toolset version in builds done with Visual Studio 2015 or later
msvc14x_toolset_ver = ''

# MSVC: Ignore warnings that aren't really harmful, but make those
#       that should not be overlooked stand out.
if is_msvc
  foreach wd : ['/FImsvc_recommended_pragmas.h', '/wd4251', '/wd4267', '/wd4275', '/wd4530', '/utf-8']
    disabled_warning = cpp_compiler.get_supported_arguments(wd)
    add_project_arguments(disabled_warning, language: 'cpp')
  endforeach
  if use_msvc14x_toolset_ver
    if cpp_compiler.version().version_compare('>=19.20')
      msvc14x_toolset_ver = '-vc142'
    elif cpp_compiler.version().version_compare('>=19.10')
      msvc14x_toolset_ver = '-vc141'
    elif cpp_compiler.version().version_compare('>=19.00')
      msvc14x_toolset_ver = '-vc140'
    else
      message('Visual Studio toolset version not applied for pre-Visual Studio 2015 builds')
    endif
  endif
endif

cairomm_libname = meson.project_name() + msvc14x_toolset_ver + '-' + cairomm_api_version

# Create cairommconfig.h.
mm_conf_data = configuration_data()
if not build_deprecated_api
  mm_conf_data.set('CAIROMM_DISABLE_DEPRECATED', 1)
endif
if build_exceptions_api
  mm_conf_data.set('CAIROMM_EXCEPTIONS_ENABLED', 1)
endif
mm_conf_data.set('CAIROMM_MAJOR_VERSION', cairomm_major_version)
mm_conf_data.set('CAIROMM_MINOR_VERSION', cairomm_minor_version)
mm_conf_data.set('CAIROMM_MICRO_VERSION', cairomm_micro_version)
mm_conf_data.set('VERSION', meson.project_version()) # for MSVC_NMake/cairomm/cairomm.rc

install_includeconfigdir = install_libdir / cairomm_pcname / 'include'
cairommconfig_h = configure_file(
  input: 'cairommconfig.h.meson',
  output: 'cairommconfig.h',
  configuration: mm_conf_data,
  install_dir: install_includeconfigdir,
)

# add_dist_script() is not allowed in a subproject if meson.version() < 0.58.0.
can_add_dist_script = not meson.is_subproject() or meson.version().version_compare('>= 0.58.0')

subdir('MSVC_NMake/cairomm')
subdir('data')
subdir('cairomm')
subdir('examples')
subdir('tests')
subdir('docs/reference')

if can_add_dist_script
  # Add a ChangeLog file to the distribution directory.
  meson.add_dist_script(
    python3.path(), dist_changelog_py,
    project_source_root,
  )
  # Add build scripts to the distribution directory, and delete .gitignore
  # files and an empty $MESON_DIST_ROOT/build/ directory.
  meson.add_dist_script(
    python3.path(), dist_build_scripts_py,
    project_source_root,
    'untracked' / 'build_scripts',
  )
endif

if meson.is_subproject()
  cairomm_dep = declare_dependency(
    dependencies: cairomm_own_dep,
    variables: {
      'doxytagfile': tag_file.full_path(),
      'htmlrefdir': install_prefix / install_docdir / 'reference' / 'html',
      'htmlrefpub': 'http://www.cairographics.org/documentation/cairomm/reference'
    }
  )

  # A main project that looks for cairomm_pcname.pc shall find cairomm_dep.
  meson.override_dependency(cairomm_pcname, cairomm_dep)
endif

# Print a summary.
real_maintainer_mode = ''
if maintainer_mode_opt == 'if-git-build'
  real_maintainer_mode = ' (@0@)'.format(maintainer_mode)
endif

real_build_documentation = ''
if build_documentation_opt == 'if-maintainer-mode'
  real_build_documentation = ' (@0@)'.format(build_documentation)
endif

real_build_tests = ''
if build_tests_opt == 'if-dependencies-found'
  real_build_tests = ' (@0@)'.format(build_tests)
endif

summary = [
  '',
  '------',
  meson.project_name() + ' ' + meson.project_version(),
  '',
  '         Maintainer mode: @0@@1@'.format(maintainer_mode_opt, real_maintainer_mode),
  '       Compiler warnings: @0@'.format(warning_level),
  '    Build deprecated API: @0@'.format(build_deprecated_api),
  'Build HTML documentation: @0@@1@'.format(build_documentation_opt, real_build_documentation),
  '  Build example programs: @0@'.format(build_examples),
  '     Build test programs: @0@@1@'.format(build_tests_opt, real_build_tests),
  '   Use shared Boost Test: @0@'.format(USE_SHARED_BOOST),
  'Directories:',
  '                  prefix: @0@'.format(install_prefix),
  '              includedir: @0@'.format(install_prefix / install_includedir),
  '       includecairommdir: @0@'.format(install_prefix / install_includedir / cairomm_pcname),
  '                  libdir: @0@'.format(install_prefix / install_libdir),
  '        includeconfigdir: @0@'.format(install_prefix / install_includeconfigdir),
  '            pkgconfigdir: @0@'.format(install_prefix / install_pkgconfigdir),
  '                 datadir: @0@'.format(install_prefix / install_datadir),
  '                  docdir: @0@'.format(install_prefix / install_docdir),
  '              devhelpdir: @0@'.format(install_prefix / install_devhelpdir),
  '------'
]

message('\n'.join(summary))
