fs = import('fs')
test_dir = meson.current_source_dir()
incs = include_directories('../include')

# Discover tests
test_srcs = run_command(
  ['fd', '.', test_dir, '--extension', 'cpp', '--extension', 'c'],
  check: true,
).stdout().strip().split('\n')

clean_tests = []
foreach s : test_srcs
  if s != ''
    clean_tests += s.strip().replace('./', '')
  endif
endforeach

# Build and register each test
foreach t_src : clean_tests
  t_name = fs.stem(fs.name(t_src))
  message('Building test: ' + t_name + ' from ' + t_src)

  t_exe = executable(
    t_name,
    t_src,
    include_directories: incs,
    dependencies: deps,
    install: false,
  )

  test('test-' + t_name, t_exe, args: ['--verbose'])
endforeach
