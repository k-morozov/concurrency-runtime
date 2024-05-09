#gtest = dependency('gtest')
#
#src = ['main.cpp', 'test_simple.cpp']
#simple = executable('simple', sources : src, dependencies : gtest)
#test('test', simple)

test_dependencies = dependency('gtest')

test_source_files = [
    'main.cpp',
    'test_simple.cpp'
]

test_executable = executable(
    'DEV-TESTS',
    test_source_files,
    dependencies : test_dependencies,
)

test('test', test_executable)