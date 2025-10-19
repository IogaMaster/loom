doxygen = find_program('doxygen', required: false)
if not doxygen.found()
  warning('Doxygen not found; skipping docs generation')
  subdir_done()
endif

doc_conf = configuration_data()
doc_conf.set('project_name', meson.project_name())

src_dir_abs = meson.project_source_root() / 'src'
inc_dir_abs = meson.project_source_root() / 'include'
theme_dir_abs = meson.project_source_root() / 'docs' / 'styles'

doc_conf.set('DOXYGEN_INPUT', src_dir_abs + ' ' + inc_dir_abs)
doc_conf.set('DOXYGEN_OUTPUT_DIR', meson.project_build_root() / 'docs')
doc_conf.set('HTML_EXTRA_STYLESHEET', theme_dir_abs / 'doxygen-awesome.css')
doc_conf.set(
  'HTML_EXTRA_FILES',
  theme_dir_abs / 'doxygen-awesome-darkmode-toggle.js'
  + ' '
  + theme_dir_abs / 'doxygen-awesome-tabs.js',
)

doxyfile = configure_file(
  input: 'Doxyfile.in',
  output: 'Doxyfile',
  configuration: doc_conf,
)

custom_target(
  'docs',
  input: doxyfile,
  output: 'docs.done',
  command: [doxygen, doxyfile],
  build_by_default: false,
)
