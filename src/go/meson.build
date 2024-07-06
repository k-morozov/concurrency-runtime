go_sources = files(
    'go.cpp',
)
go = library('go',
             go_sources,
             include_directories : [root_src_dir],
             link_with : [fiber, coro, context, components],
)