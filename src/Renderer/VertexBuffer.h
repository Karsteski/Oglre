#pragma once

#include <cstdint>
#include <vector>

// Handles creation, binding, data storage and deletion of a Vertex Buffer Object.
class VertexBuffer {
public:
    VertexBuffer(const std::vector<float> data, uint32_t size);
    ~VertexBuffer();

    void Bind();
    void Unbind();

private:
    uint32_t m_RendererID;
};
