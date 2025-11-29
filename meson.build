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
  # Core deps
  dependency('sol2', default_options: ['use_luajit=true']),
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

# --------- Module Build ---------

module_names = []
foreach src : clean_srcs
  if not src.contains('src/modules')
    continue
  endif
  module_name = src.split('/')[-2]
  if not module_names.contains(module_name)
    module_names += module_name
  endif
endforeach

foreach module_name : module_names
  module_srcs = []
  foreach src : clean_srcs
    if src.startswith(module_name)
      module_srcs += src
    endif
  endforeach

  # Create the shared library for this module
  shared_library(
    'loom_' + module_name,
    module_srcs,
    include_directories: include_directories('include'),
    install: false,
    dependencies: deps,
  )
endforeach

# --------- Runtime ---------

executable(
  'loom',
  ['src/main.cpp'],
  include_directories: incs,
  dependencies: deps,
  install: true,
)

# --------- Subdirectories ---------

subdir('docs')
subdir('tests')
