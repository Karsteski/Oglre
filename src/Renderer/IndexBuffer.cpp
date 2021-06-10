#include "IndexBuffer.h"
#include <GL/glew.h>
#include <cstdint>

Oglre::IndexBuffer::IndexBuffer(const std::vector<uint32_t> data)
    : m_Count(data.size())
{
    const int numberOfBuffers = 1;

    glGenBuffers(numberOfBuffers, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(uint32_t), data.data(), GL_STATIC_DRAW);
}

Oglre::IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void Oglre::IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void Oglre::IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
