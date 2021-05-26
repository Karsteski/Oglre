project('oglre','cpp', version : '0.1.0')

# Not the Mesonic way to do this, but default_options : cpp_std=c++17 isn't working for some reason.
add_global_arguments('-std=c++17', language : 'cpp')

glew_dep = dependency('glew')
glfw_dep = dependency('glfw3')
opengl_dep = dependency('opengl')
imgui_dep = dependency('imgui', fallback : ['imgui', 'imgui_dep'])

src_files = [
    'src/main.cpp',
    'src/Application/Application.cpp',
    'src/Renderer/VertexBuffer.cpp',
    'src/Renderer/IndexBuffer.cpp',
    'src/Renderer/VertexArray.cpp',
    'src/Renderer/Renderer.cpp',
    'src/Shader/Shader.cpp'
]

include_dirs = [
    'src/Application',
    'src/Renderer',
    'src/Shader'
]

executable('oglre',
    sources : src_files,
    dependencies : [glew_dep, glfw_dep, opengl_dep, imgui_dep],
    include_directories : include_dirs
)