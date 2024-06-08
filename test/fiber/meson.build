test_source_files = [
    'main.cpp',
    'test_fiber.cpp',
    'test_mutex.cpp',
]

test_fiber = executable(
    'test_fiber',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, runtime_inc],
    link_with : [context, coro, executor, fiber, go]
)

test('test_fiber', test_fiber)