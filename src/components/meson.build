components_sources = files(
    'sync/spinlock.cpp',
    'queue/blocking_queue.h',
    'intrusive/list.cpp',
    'function/basic_function.h',
    'lock_free/classic_ms_queue.cpp',
    'lock_free/simple_ms_queue.cpp',
)

cpp = meson.get_compiler('cpp')
threads_dep = dependency('threads')
libatomic_dep = cpp.find_library('atomic', required : true)

components = library('components',
                     dependencies : [threads_dep, libatomic_dep],
                     sources: components_sources,
)