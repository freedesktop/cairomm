# tests

# input: cairomm_own_dep, build_tests, can_test, test_dep, USE_SHARED_BOOST

cairomm_tests_extra_cppargs = []
if USE_SHARED_BOOST
  cairomm_tests_extra_cppargs += ['-DBOOST_ALL_DYN_LINK']
endif

test_programs = [
# [[dir-name], exe-name, [sources], [cpp-flags]]
  [[], 'test-context', ['test-context.cc'], []],
  [[], 'test-font-face', ['test-font-face.cc'], []],
  [[], 'test-font-options', ['test-font-options.cc'], []],
  [[], 'test-matrix', ['test-matrix.cc'], []],
  [[], 'test-scaled-font', ['test-scaled-font.cc'], []],
  [[], 'test-surface', ['test-surface.cc'],
    ['-DPNG_STREAM_FILE="' + meson.current_source_dir() / 'png-stream-test.png"']],
  [[], 'test-user-font', ['test-user-font.cc'], []],
]

if can_test
  foreach ex : test_programs
    dir = ''
    foreach dir_part : ex[0]
      dir = dir / dir_part
    endforeach
    ex_name = (dir / ex[1]).underscorify()
    ex_sources = []
    foreach src : ex[2]
      ex_sources += dir / src
    endforeach

    boost_test_modname_flag = [ '-DBOOST_TEST_MODULE=@0@'.format(ex[1]) ]
    exe_file = executable(ex_name, ex_sources,
      cpp_args: ex[3] + boost_test_modname_flag + cairomm_tests_extra_cppargs,
      dependencies: [cairomm_own_dep, test_dep],
      implicit_include_directories: false,
      gui_app: false,
      build_by_default: build_tests,
    )

    # If exe_file is a test program, it is built by default unconditionally.
    if build_tests
      test(ex_name, exe_file)
    endif
  endforeach
endif
