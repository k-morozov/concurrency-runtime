fiber_sources = files(
    'fiber.cpp',
    'awaiter_fiber.cpp',
    'handle/stopped_fiber.cpp',
    'awaiter/suspend.cpp',
    'sync/async_mutex.cpp',
    'sync/wait_group.cpp',
    'sync/event.cpp',
    'intrusive/list.cpp',
)
fiber = library('fiber',
                fiber_sources,
                include_directories : [runtime_inc],
                link_with : [coro, context, executor, components],
)