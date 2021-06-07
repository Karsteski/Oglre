#include "VertexBuffer.h"
#include <GL/glew.h>

Oglre::VertexBuffer::VertexBuffer(const std::vector<float> data, uint32_t size)
{
    const int numberOfBuffers = 1;

    glGenBuffers(numberOfBuffers, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
}

Oglre::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void Oglre::VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void Oglre::VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
