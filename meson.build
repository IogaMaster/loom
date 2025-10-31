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

fs = import('fs')

src_dir = 'src'
incs = include_directories('include')

deps = [
  dependency('sdl3'),
  dependency('sdl3_image'),
  dependency('sdl3_ttf'),
  dependency('imgui-docking'),
]

# --------- Source Discovery ---------

# Meson says this is not a good idea, because you need to rerun `meson setup` on each new file
# But you have to manually add the file each time, which means you need to run `meson setup` again anyway
# So I cannot be asked to rewrite it each time and fd is fast enough, so here it is. Just as unintended! ;)
srcs = run_command(
  ['fd', '.', src_dir, '--extension', 'cpp', '--extension', 'c'],
  check: true,
).stdout().strip().split('\n')

clean_srcs = []
foreach s : srcs
  if s != ''
    clean_srcs += s.strip().replace('./', '')
  endif
endforeach

# --------- Library Build ---------

loom_lib = shared_library(
  'loom',
  clean_srcs,
  include_directories: incs,
  install: false,
  dependencies: deps,
)

loom_dep = declare_dependency(
  link_with: loom_lib,
  include_directories: incs,
  dependencies: deps,
)
meson.override_dependency('loom', loom_dep)

# --------- Subdirectories ---------

subdir('examples')
subdir('docs')
subdir('tests')
