bench_inc = include_directories('.')

#gbenchmark_dep = benchmark.get_variable('google_benchmark_dep')

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
    include_directories : [bench_inc, runtime_inc],
    link_with : [executor, fiber, go]
)


test('bench_pool', bench_pool)
