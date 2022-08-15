
project(
    'relptr',
    'cpp',
    default_options : [
        'cpp_std=c++20',
        'warning_level=3'
    ],
)

src = [ 'src/main.cpp' ]
include_dir = 'include/'

exe = executable(
    'relptr',
    sources : src,
    include_directories : include_dir,
    cpp_args : [ '-g' ],
    override_options : [
        'build_type=release'
    ]
)

exe_d = executable(
    'relptr_d',
    sources : src,
    include_directories : include_dir,
    cpp_args : [ '-g' ]
)