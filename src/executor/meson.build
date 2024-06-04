executor_sources = files(
    'pool/thread_pool.cpp',
    'manual/manual_executor.cpp',
    'task/task.cpp',
)

executor = library('executor',
                   executor_sources,
                   install : true,
                   include_directories : [runtime_inc],
                   link_with : [context, pool_queue],
)