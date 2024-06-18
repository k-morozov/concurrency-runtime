go_sources = files(
    'go.cpp',
)
go = library('go',
             go_sources,
             include_directories : [runtime_inc],
             link_with : [fiber, coro, context, components],
)