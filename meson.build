project('oglre','cpp', version : '0.1.0')

# Print relevant options.
message('C++ Version = ' + get_option('cpp_std'))
message('Warning level = ' + get_option('warning_level'))
message('Build type = ' + get_option('buildtype'))

glew_dep = dependency('glew', fallback : ['glew', 'glew_dep'])
glfw_dep = dependency('glfw3')
opengl_dep = dependency('opengl')
imgui_dep = dependency('imgui', fallback : ['imgui', 'imgui_dep'])
glm_dep = dependency('glm', fallback: ['glm', 'glm_dep'])

src_files = [
    'src/Main.cpp',
    'src/Application/Application.cpp',
    'src/Renderer/VertexBuffer.cpp',
    'src/Renderer/IndexBuffer.cpp',
    'src/Renderer/VertexArray.cpp',
    'src/Renderer/Renderer.cpp',
    'src/Shader/Shader.cpp',
    'src/Camera/Camera.cpp'
]

include_dirs = [
    'src/Application',
    'src/Renderer',
    'src/Shader',
    'src/Camera'
]

executable('oglre',
    sources : src_files,
    dependencies : [glew_dep, glfw_dep, opengl_dep, imgui_dep, glm_dep],
    include_directories : include_dirs
)
