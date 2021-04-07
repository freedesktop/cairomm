# examples

# input: cairomm_own_dep, build_examples

examples = [
# [[dir-name], exe-name, [sources]]
  [['surfaces'], 'image-surface', ['image-surface.cc']],
  [['surfaces'], 'pdf-surface', ['pdf-surface.cc']],
  [['surfaces'], 'ps-surface', ['ps-surface.cc']],
  [['surfaces'], 'svg-surface', ['svg-surface.cc']],
  [['text'], 'text-rotate', ['text-rotate.cc']],
  [['text'], 'toy-text', ['toy-text.cc']],
  [['text'], 'user-font', ['user-font.cc']],
]

foreach ex : examples
  dir = ''
  foreach dir_part : ex[0]
    dir = dir / dir_part
  endforeach
  ex_name = (dir / ex[1]).underscorify()
  ex_sources = []
  foreach src : ex[2]
    ex_sources += dir / src
  endforeach

  executable(ex_name, ex_sources,
    cpp_args: '-DCAIROMM_DISABLE_DEPRECATED',
    dependencies: cairomm_own_dep,
    implicit_include_directories: false,
    gui_app: false,
    build_by_default: build_examples
  )
endforeach
