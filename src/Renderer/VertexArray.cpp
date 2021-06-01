#include <cstdint>

#include "Renderer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}
VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // First bind Vertex Array.
    Bind();

    // Then bind buffer.
    vb.Bind();

    // Set up layout for the buffer.
    const auto& elements = layout.GetElements();

    uint32_t offset = 0;
    uint32_t colourOffset = 3 * sizeof(float);


    // TODO: Need to add support for multiple Vertex Array Objects.
    for (uint32_t index = 0; index < elements.size(); ++index) {
        const auto& element = elements[index];

        // Vertex position attribute.
        // Note the necessary void* cast due to the OpenGL API.
        glVertexAttribPointer(0, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
        // Must enable the generic vertex attribute array for the vertex to be drawn.
        glEnableVertexAttribArray(0);

        // Vertex colour attribute.
        glVertexAttribPointer(1, element.count, element.type, element.normalized, layout.GetStride(), (void*)colourOffset);
        glEnableVertexAttribArray(1);

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        colourOffset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}