executor_sources = files(
    'pool/intrusive_pool.cpp',
    'pool/distributed_pool.cpp',
    'pool/internal/worker.cpp',
    'manual/intrusive_manual_executor.cpp',
    'strand/intrusive_strand.cpp',
    'task/task.cpp',
    'task/internal/task_free_function.cpp',
    'submit.cpp',
)

cpp = meson.get_compiler('cpp')
threads_dep = dependency('threads')
libatomic_dep = cpp.find_library('atomic', required : true)

executor = library('executor',
                   executor_sources,
                   install : true,
                   include_directories : [root_src_dir],
                   dependencies : [threads_dep, libatomic_dep],
                   link_with : [context, coro, components],
)