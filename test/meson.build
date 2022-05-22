gtest_dep = dependency('gtest')

testexe = executable('fifo_test', 'main.cpp',
                     include_directories : inc,
                     link_with : libfifo,
                     dependencies: gtest_dep)
test('fifo test', testexe)