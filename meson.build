project(
  'loom',
  ['c', 'cpp'],
  license: 'MIT',
  license_files: 'LICENSE',
  default_options: [
    'default_library=static',
    'warning_level=3',
    'c_std=c17',
    'cpp_std=c++20',
    'cpp_args=-O2 -Wall -Wextra -fdiagnostics-color=always',
  ],
)

src_dir = 'src'
examples_dir = 'examples'
incs = include_directories('include')

deps = [
  dependency('sdl3'),
  dependency('sdl3_image'),
  dependency('sdl3_ttf'),
]

fs = import('fs')

# Meson says this is not a good idea, because you need to rerun `meson setup` on each new file
# But you have to manually add the file each time, which means you need to run `meson setup` again anyway
# So I cannot be asked to rewrite it each time and fd is fast enough, so here it is. Just as unintended! ;)
srcs = run_command(['fd', '.', src_dir, '--extension', 'cpp', '--extension', 'c'], check: true).stdout().strip().split('\n')
examples_srcs = run_command(
  ['fd', '.', examples_dir, '--extension', 'cpp', '--extension', 'c'],
  check: true,
).stdout().strip().split('\n')

clean_srcs = []
foreach s : srcs
  if s != ''
    clean_srcs += s.strip().replace('./', '')
  endif
endforeach

clean_examples = []
foreach s : examples_srcs
  if s != ''
    clean_examples += s.strip().replace('./', '')
  endif
endforeach

# --------- Final Build ---------

loom_lib = library(
  'loom',
  clean_srcs,
  include_directories: incs,
  install: false,
  dependencies: deps,
)

foreach ex_src : clean_examples
  ex_name = fs.stem(fs.name(ex_src))
  message('Building example: ' + ex_name + ' from ' + ex_src)
  executable(
    ex_name,
    ex_src,
    include_directories: incs,
    link_with: loom_lib,
    dependencies: deps,
    install: false,
  )
endforeach
