test_source_files = [
    'main.cpp',
    'test_thread_pool.cpp',
    'test_manual_executor.cpp',
    'test_strand.cpp',
#    'test_distributed_pool.cpp',
]

test_pool = executable(
    'test_pool',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, runtime_inc],
    link_with : [executor, fiber, go]
)


test('test_pool', test_pool)
