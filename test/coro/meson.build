test_source_files = [
    'main.cpp',
    'test_coro.cpp',
    'test_coro_threads.cpp'
]

test_coro = executable(
    'test_coro',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, runtime_inc],
    link_with : [context, coro]
)

test('test', test_coro)