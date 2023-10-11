add_requires('mini-unit-test')

target('check', function()
  set_kind('binary')
  add_files('*.c')
  set_default('false')

  add_packages('mini-unit-test')

  add_deps('u', 'macro')
end)

task('check', function()
  set_menu({
    usage = 'xmake check',
    description = 'Run check',
  })

  on_run(function()
    import('core.project.project')
    import('lib.detect.find_program')

    local target = project.target('check')
    local bin_path = target:targetdir() .. '/valgrind/' .. target:name()

    os.exec('xmake f -m valgrind --mimalloc=n')
    os.exec('xmake build -v check')

    if find_program('valgrind') ~= nil then
      local cmd = format('valgrind --tool=memcheck --leak-check=full %s', bin_path)
      os.exec(cmd)
    else
      os.exec('xmake run -v check')
    end
  end)
end)
