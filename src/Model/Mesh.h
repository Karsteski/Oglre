#pragma once

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace Oglre {

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh() = default;

    const VertexArray& GetVertexArrayObject() const;
    const VertexBuffer& GetVertexBufferObject() const;
    const IndexBuffer& GetIndexBufferObject() const;

private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;

    void SetupMesh();
};

}