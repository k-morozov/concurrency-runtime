test_source_files = [
    'main.cpp',
    'test_ctx.cpp'
]

test_ctx = executable(
    'test_ctx',
    sources : test_source_files,
    dependencies : test_dependencies,
    include_directories : [test_inc, runtime_inc],
    link_with : context,
)

test('test_ctx', test_ctx)