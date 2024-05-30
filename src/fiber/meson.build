fiber_sources = files(
    'fiber.cpp',
)
fiber = library('fiber',
                fiber_sources,
                include_directories : [runtime_inc],
                link_with : [coro, context, pool_queue, executor],
)