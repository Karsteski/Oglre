#pragma once

#include <cstdint>

class VertexBuffer {
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void Bind();
    void Unbind();

private:
    uint32_t m_RendererID;
};
