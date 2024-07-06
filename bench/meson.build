bench_inc = include_directories('.')

bench_source_files = [
    'main.cpp',
    'bench_pool.cpp',
]

bench_dependencies = dependency(
    [
        'benchmark',
    ]
)

bench_pool = executable(
    'bench_pool',
    sources : bench_source_files,
    dependencies : bench_dependencies,
    include_directories : [bench_inc, root_src_dir],
    link_with : [executor, fiber, go]
)
