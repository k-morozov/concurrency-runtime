components_sources = files(
    'sync/spinlock.cpp',
    'queue/blocking_queue.h',
    'intrusive/list.cpp',
    'function/basic_function.h',
    'lock_free/classic_ms_queue.cpp',
    'lock_free/simple_ms_queue.cpp',
    'lock_free/hazard/mutator.cpp',
    'lock_free/hazard/hazard_manager.cpp',
    'lock_free/hazard/thread_state.cpp',
    'async_mutex/async_mutex.cpp',
    'async_mutex/async_mutex_coro_impl.cpp',
    'async_mutex/mutex_awaiter.cpp',
    'async_mutex/resumable_no_own.cpp',
)

cpp = meson.get_compiler('cpp')
threads_dep = dependency('threads')
libatomic_dep = cpp.find_library('atomic', required : true)

components = library('components',
                     dependencies : [threads_dep, libatomic_dep],
                     include_directories : [root_src_dir],
                     sources: components_sources,
)