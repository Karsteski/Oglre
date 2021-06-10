#pragma once

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <cstdint>
#include <string>
#include <vector>
namespace Oglre {

struct Vertex {
    glm::vec3 position;
    glm::vec3 colour;
};

enum class Attribute {
    POSITION,
    COLOUR
};

// Handles creation, binding, data storage and deletion of a Vertex Buffer Object.
class VertexBuffer {
public:
    VertexBuffer(const std::vector<Vertex>& vertices);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    // Query number of floats in a particular attribute.
    int GetFloatsInAttribute(Attribute attribute) const;

private:
    uint32_t m_RendererID;
};
}