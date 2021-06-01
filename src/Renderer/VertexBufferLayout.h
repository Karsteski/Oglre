#pragma once

#include <GL/glew.h>
#include <cstddef>
#include <vector>

struct VertexBufferElement {
    uint32_t type;
    uint32_t count;

    // unsigned char rather than bool to avoid Glboolean conversion.
    unsigned char normalized;

    static uint32_t GetSizeOfType(uint32_t type)
    {
        // clang-format off
        switch (type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        // clang-format on

        return 0;
    }
};

// Maintains elements in the Vertex Buffer, specifying the layout of the buffer.
// Index of element is the index in the m_Elements std::vector.
class VertexBufferLayout {
public:
    VertexBufferLayout()
        : m_Stride(0) {};

    template <typename T>
    // Explicitly prevent unspecified function from being called.
    // Templates are outside class as C++ standard does not allow for explicit template specialization
    // in a non-namespace scope.
    void Push(uint32_t count) = delete;

    // Long explanation, see: http://docs.gl/gl4/glVertexAttribPointer
    inline uint32_t GetStride() const
    {
        return m_Stride;
    }

    inline const std::vector<VertexBufferElement> GetElements() const
    {
        return m_Elements;
    }

private:
    std::vector<VertexBufferElement> m_Elements;

    uint32_t m_Stride;
};

// Specializations for each type of element.
template <>
inline void VertexBufferLayout::Push<float>(uint32_t count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride = count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template <>
inline void VertexBufferLayout::Push<uint32_t>(uint32_t count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride = count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template <>
inline void VertexBufferLayout::Push<unsigned char>(uint32_t count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride = count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}