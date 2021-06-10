#include "Mesh.h"

Oglre::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : m_VAO()
    , m_VBO(vertices)
    , m_IBO(indices)
{
    SetupMesh();
}

const Oglre::VertexArray& Oglre::Mesh::GetVertexArrayObject() const
{
    return m_VAO;
}

const Oglre::VertexBuffer& Oglre::Mesh::GetVertexBufferObject() const
{
    return m_VBO;
}

const Oglre::IndexBuffer& Oglre::Mesh::GetIndexBufferObject() const
{
    return m_IBO;
}

void Oglre::Mesh::SetupMesh()
{
    // Create Vertex Buffer Layout.
    VertexBufferLayout layout;

    // One Push for each vertex attribute.
    layout.Push<float>(m_VBO.GetFloatsInAttribute(Attribute::POSITION));
    layout.Push<float>(m_VBO.GetFloatsInAttribute(Attribute::COLOUR));
    m_VAO.AddBuffer(m_VBO, layout);
}
