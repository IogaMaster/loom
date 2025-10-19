fs = import('fs')
examples_dir = meson.current_source_dir()
incs = include_directories('../include')

deps = [
  dependency('sdl3'),
  dependency('sdl3_image'),
  dependency('sdl3_ttf'),
  dependency('loom'),
]

# Discover example sources
examples_srcs = run_command(
  ['fd', '.', '--extension', 'cpp', '--extension', 'c'],
  check: true,
).stdout().strip().split('\n')

clean_examples = []
foreach s : examples_srcs
  if s != ''
    clean_examples += s.strip().replace('./', '')
  endif
endforeach

# Build each example as an executable
foreach ex_src : clean_examples
  ex_name = fs.stem(fs.name(ex_src))
  message('Building example: ' + ex_name + ' from ' + ex_src)

  executable(
    ex_name,
    ex_src,
    include_directories: incs,
    dependencies: deps,
    install: false,
  )
endforeach
