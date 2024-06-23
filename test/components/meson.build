test_source_files = [
    'main.cpp',
    'test_unbounded_blocking_queue.cpp',
    'test_basic_function.cpp',
    'test_intrusive_list.cpp',
]

test_components = executable(
    'test_components',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, runtime_inc],
    link_with : executor,
)

test('test_components', test_components)