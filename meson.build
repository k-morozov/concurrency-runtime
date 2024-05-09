project('go-runtime', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++20'])

go_runtime = executable('go_runtime', 'main.cpp', install : true)

test('test', go_runtime)