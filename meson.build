project('oglre','cpp', version : '0.1.0', default_options : 'cpp_std=c++17')

add_global_arguments('-std=c++17', language : 'cpp')

glew_dep = dependency('glew')
glfw_dep = dependency('glfw3')
opengl_dep = dependency('opengl')
imgui_dep = dependency('imgui', fallback : ['imgui', 'imgui_dep'])

src_files = [
    'src/main.cpp'
]

include_dirs = []

executable('oglre',
    sources : src_files,
    dependencies : [glew_dep, glfw_dep, opengl_dep, imgui_dep],
    include_directories : include_dirs,
)