#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <cstdint>

namespace Oglre {

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    // Binds Vertex Buffer and sets up the layout.
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;

private:
    uint32_t m_RendererID;
};
}