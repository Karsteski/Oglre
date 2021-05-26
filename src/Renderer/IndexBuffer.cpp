#include "IndexBuffer.h"
#include <GL/glew.h>
#include <cstdint>

IndexBuffer::IndexBuffer(const std::vector<uint32_t> data, uint32_t count)
    : m_Count(count)
{
    const int numberOfBuffers = 1;

    glGenBuffers(numberOfBuffers, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
