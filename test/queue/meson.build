test_source_files = [
    'main.cpp',
    'test_unbounded_blocking_queue.cpp'
]

test_queue = executable(
    'test_queue',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, runtime_inc],
    link_with : executor,
)

test('test_queue', test_queue)