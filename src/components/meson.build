components_sources = files(
    'sync/spinlock.cpp',
    'queue/blocking_queue.h',
    'intrusive/list.cpp',
    'function/basic_function.h',
    'lock_free/classic_ms_queue.cpp',
    'lock_free/simple_ms_queue.cpp',
    'lock_free/hazard/hazard.cpp',
    'lock_free/hazard/thread_state.cpp',
    'lock_free/hazard/scan.cpp',
)

cpp = meson.get_compiler('cpp')
threads_dep = dependency('threads')
libatomic_dep = cpp.find_library('atomic', required : true)

components = library('components',
                     dependencies : [threads_dep, libatomic_dep],
                     include_directories : [root_src_dir],
                     sources: components_sources,
)