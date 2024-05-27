context_sources = files(
    'context.cpp',
    'buffer/buffer.cpp',
    'buffer/prepare_context.s',
    'buffer/buffer_context.cpp',
)
context = library('context',
                  context_sources,
                  include_directories : [runtime_inc],
)