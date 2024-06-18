coro_sources = files(
    'coro.cpp',
    'stackfull_coro.cpp',
)
coro = library('coro',
               coro_sources,
               include_directories : [runtime_inc],
               link_with : [context, components],
)