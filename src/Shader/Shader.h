#pragma once

#include <GL/glew.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform(const std::string& name, float f0, float f1, float f2, float f4);
    void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

    // Takes care of returning the two strings from parseShader().
    struct ShaderProgramSource {
        std::string vertexSource;
        std::string fragmentSource;
    };

private:
    uint32_t m_RendererID;

    // For debugging purposes.
    std::string m_FilePath;

    // For caching the uniform location.
    // Constantly retrieving the uniform location is slow and unnecessary, hence the cache.
    std::unordered_map<std::string, int> m_UniformLocationCache;

    ShaderProgramSource ParseShader(const std::string& filepath);

    // Ensure that source string does not go out of scope before running compileShader().
    uint32_t CompileShader(uint32_t type, const std::string& source);

    // Returns ID for a shader that combines the vertexShader and the fragmentShader.
    uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    uint32_t GetUniformLocation(const std::string& name);
};