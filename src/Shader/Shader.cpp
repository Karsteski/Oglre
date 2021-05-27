#include <array>
#include <cstdint>

#include "Shader.h"

Shader::Shader(const std::string& filepath)
    : m_RendererID(0)
    , m_FilePath(filepath)
{
    ShaderProgramSource shaderSource = ParseShader(filepath);
    m_RendererID = CreateShader(shaderSource.vertexSource, shaderSource.fragmentSource);
}
Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}
void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
    const int nElements = 1;
    glUniformMatrix4fv(GetUniformLocation(name), nElements, GL_FALSE, &matrix[0][0]);
}

Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    // For separating the two stringstreams.
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::array<std::stringstream, 2> ss;

    std::string line = "";
    ShaderType type = ShaderType::NONE;

    // Read lines from the file while separating the two shader types into different stringstreams.
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    ShaderProgramSource shaders;
    shaders.vertexSource = ss[0].str();
    shaders.fragmentSource = ss[1].str();

    return shaders;
}

uint32_t Shader::CompileShader(uint32_t shaderType, const std::string& source)
{
    unsigned int id = glCreateShader(shaderType);

    // Note that source has to exist
    const char* src = source.c_str();

    const int numberOfShaderSources = 1;
    // Note that strings must be null-terminated if passing length as nullptr.
    glShaderSource(id, numberOfShaderSources, &src, nullptr);
    glCompileShader(id);

    // Error handling.
    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int errorMessageLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorMessageLength);
        std::string message = "";
        glGetShaderInfoLog(id, errorMessageLength, &errorMessageLength, message.data());

        // A bit hacky, will eventually need proper logging.
        std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n"
                  << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Need error handling.
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // These steps create an executable that is run on the programmable vertex/fragment shader processer on the GPU.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Delete to shaders once they have been linked and compiled.
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

uint32_t Shader::GetUniformLocation(const std::string& name)
{
    // If uniform location has not been cached.
    if (m_UniformLocationCache.find(name) == m_UniformLocationCache.end()) {
        int location = glGetUniformLocation(m_RendererID, name.c_str());
        // Sometimes a uniform location can be -1 if unused, for example.
        if (location == -1) {
            std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;
        }

        // Cache location for later.
        m_UniformLocationCache[name] = location;
        return location;
    } else {
        // Uniform location already exists.
        return m_UniformLocationCache[name];
    }
}