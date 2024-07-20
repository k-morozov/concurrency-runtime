test_source_files = [
    'main.cpp',
    'test_unbounded_blocking_queue.cpp',
    'test_basic_function.cpp',
    'test_intrusive_list.cpp',
    'test_ms_queue.cpp',
    'test_async_mutex.cpp',
]

cpp = meson.get_compiler('cpp')
threads_dep = dependency('threads')
libatomic_dep = cpp.find_library('atomic', required : true)

test_components = executable(
    'test_components',
    sources : test_source_files,
    dependencies : [threads_dep, libatomic_dep, test_dependencies],
    include_directories : [test_inc, root_src_dir],
    link_with : [components],
)

test('test_components', test_components)