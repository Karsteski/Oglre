#include "VertexBuffer.h"
#include <GL/glew.h>
#include <cstdint>

VertexBuffer::VertexBuffer(const std::vector<float> data, uint32_t size)
{
    const int numberOfBuffers = 1;
    
    glGenBuffers(numberOfBuffers, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    //glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
