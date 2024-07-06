coro_sources = files(
    'coro.cpp',
    'stackfull_coro.cpp',
)
coro = library('coro',
               coro_sources,
               include_directories : [root_src_dir],
               link_with : [context, components],
)