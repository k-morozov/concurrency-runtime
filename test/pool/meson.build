test_source_files = [
    'main.cpp',
    'test_simple.cpp'
]

test_executable = executable(
    'test_executor',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [runtime_inc],
    link_with : executor,
)


test('test', test_executable)
