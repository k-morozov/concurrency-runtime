executor_sources = files(
    'pool/thread_pool.cpp',
    'pool/intrusive_pool.cpp',
    'manual/manual_executor.cpp',
    'manual/intrusive_manual_executor.cpp',
    'strand/strand.cpp',
    'strand/intrusive_strand.cpp',
    'task/task.cpp',
    'task/internal/task_free_function.cpp',
    'submit.cpp',
)

executor = library('executor',
                   executor_sources,
                   install : true,
                   include_directories : [runtime_inc],
                   link_with : [context, components],
)