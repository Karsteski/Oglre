#include "VertexBuffer.h"
#include <GL/glew.h>

Oglre::VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
{
    constexpr int numberOfBuffers = 1;

    glGenBuffers(numberOfBuffers, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
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

int Oglre::VertexBuffer::GetFloatsInAttribute(Attribute attribute) const
{
    int nFloats = 0;

    switch (attribute) {
    case Attribute::POSITION: {
        nFloats = 3;
        break;
    }

    case Attribute::COLOUR: {
        nFloats = 3;
        break;
    }
    }

    return nFloats;
}