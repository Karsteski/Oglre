// clang-format off
// Cube positions.
    std::vector<float> vertices = {
        // Positions                // Colours
        100.0f, 100.0f, 100.0f,     1.0f, 0.0f, 0.0f, // 0
        -100.0f, 100.0f, 100.0f,    0.0f, 1.0f, 0.0f, // 1
        -100.0f, 100.0f, -100.0f,   0.0f, 0.0f, 1.0f, // 2
        100.0f, 100.0f, -100.0f,    1.0f, 0.0f, 0.0f, // 3
        100.0f, -100.0f, 100.0f,    0.0f, 1.0f, 0.0f, // 4
        -100.0f, -100.0f, 100.0f,   0.0f, 0.0f, 1.0f, // 5
        -100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 0.0f, // 6
        100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 0.0f  // 7
    };

// Specializations for each type of element.
template <>
inline void VertexBufferLayout::Push<float>(uint32_t count)
{
    // m_Elements is a std::vector<float>
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride = count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

// Instructing OpenGL to use the layout passed to address my Vertex Buffer data.
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


    for (uint32_t index = 0; index < elements.size(); ++index) {
        const auto& element = elements[index];

        // Vertex position attribute.
        // Note the necessary void* cast due to the OpenGL API.
        glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
        // Must enable the generic vertex attribute array for the vertex to be drawn.
        glEnableVertexAttribArray(index);

        // Vertex colour attribute.
        glVertexAttribPointer(index, element.count, element.type, element.normalized, layout.GetStride(), (void*)colourOffset);
        glEnableVertexAttribArray(index);

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        colourOffset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}