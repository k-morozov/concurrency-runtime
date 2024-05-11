executor_sources = files(
    'executor/executor.cpp',
)

executor = library('executor',
                   executor_sources,
                   install : true,
                   include_directories : [runtime_inc],
                   link_with : pool_queue,
)