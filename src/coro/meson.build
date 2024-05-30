coro_sources = files(
    'coro.cpp',
    'fiber_coro.cpp',
)
coro = library('coro',
               coro_sources,
               include_directories : [runtime_inc],
               link_with : [context, pool_queue],
)