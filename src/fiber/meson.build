fiber_sources = files(
    'fiber.cpp',
    'fiber_handle.cpp',
    'awaiter_fiber.cpp',
    'sync/async_mutex.cpp',
)
fiber = library('fiber',
                fiber_sources,
                include_directories : [runtime_inc],
                link_with : [coro, context, pool_queue, executor],
)