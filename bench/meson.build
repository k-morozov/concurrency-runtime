bench_inc = include_directories('.')

bench_pool_source_files = [
    'main.cpp',
    'bench_pool.cpp',
]

bench_mutex_source_files = [
    'main.cpp',
    'bench_async_mutex.cpp',
]

bench_dependencies = dependency(
    [
        'benchmark',
    ]
)

bench_pool = executable(
    'bench_pool',
    sources : bench_pool_source_files,
    dependencies : bench_dependencies,
    include_directories : [bench_inc, root_src_dir],
    link_with : [executor, fiber, go]
)

bench_async_mutex = executable(
    'bench_async_mutex',
    sources : bench_mutex_source_files,
    dependencies : bench_dependencies,
    include_directories : [bench_inc, root_src_dir],
    link_with : [components]
)