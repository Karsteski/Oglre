project('oglre','cpp', version : '0.1.0')

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