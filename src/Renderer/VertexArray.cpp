#include <cstdint>

#include "Renderer.h"
#include "VertexArray.h"

Oglre::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}
Oglre::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void Oglre::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // First bind Vertex Array.
    Bind();

    // Then bind buffer.
    vb.Bind();

    // Set up layout for the buffer.
    const auto& elements = layout.GetElements();

    uint32_t offset = 0;
    
    // Loop through vertex attributes.
    for (uint32_t index = 0; index < elements.size(); ++index) {
        const auto& element = elements[index];

        // Vertex Attribute
        // Note the necessary void* cast due to the OpenGL API.
        glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
        // Must enable the generic vertex attribute array for the vertex to be drawn.
        glEnableVertexAttribArray(index);

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void Oglre::VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void Oglre::VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
