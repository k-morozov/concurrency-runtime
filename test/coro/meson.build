test_source_files = [
    'main.cpp',
    'test_coro.cpp',
    'test_coro_threads.cpp'
]

test_coro = executable(
    'test_coro',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, root_src_dir],
    link_with : [context, coro]
)

test('test_coro', test_coro)