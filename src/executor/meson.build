executor_sources = files(
    'pool/thread_pool.cpp',
    'manual/manual_executor.cpp',
    'strand/strand.cpp',
    'task/task.cpp',
    'submit.cpp',
)

executor = library('executor',
                   executor_sources,
                   install : true,
                   include_directories : [runtime_inc],
                   link_with : [context, components],
)