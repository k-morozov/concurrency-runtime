test_source_files = [
    'main.cpp',
    'test_simple.cpp'
]

test_thread_pool = executable(
    'test_thread_pool',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [runtime_inc],
    link_with : executor,
)


test('test_thread_pool', test_thread_pool)
