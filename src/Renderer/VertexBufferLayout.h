#pragma once

#include <GL/glew.h>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <vector>

struct VertexBufferElement {
    uint32_t type;
    uint32_t count;
    bool normalized;
};

// Maintains elements in the Vertex Buffer, specifying the layout of the buffer.
// Index of element is the index in the m_Elements std::vector.
class VertexBufferLayout {
public:
    VertexBufferLayout()
        : m_Stride(0) {};

    template <typename T>
    // Explicitly prevent unspecified function from being called.
    void Push(uint32_t count) = delete;

    template <>
    void Push<float>(uint32_t count)
    {
        m_Elements.push_back({ GL_FLOAT, count, false });
        m_Stride += sizeof(float);
    }

    template <>
    void Push<uint32_t>(uint32_t count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
        m_Stride += sizeof(uint32_t);
    }

    template <>
    void Push<unsigned char>(uint32_t count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, false });
        m_Stride += sizeof(unsigned char);
    }

    inline uint32_t GetStride() const
    {
        return m_Stride;
    }

private:
    std::vector<VertexBufferElement> m_Elements;

    uint32_t m_Stride;
};