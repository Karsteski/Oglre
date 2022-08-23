#include "Renderer.h"

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const Oglre::VertexArray& va, const Oglre::IndexBuffer& ibo, const Shader& shader)
{
    shader.Bind();
    va.Bind();

    // TODO: Don't need to bind an ibo again. VAOs keep track of the last IBO bound, so what we really need is to keep track of what the last IBO + VBO was about to a VAO 
    ibo.Bind();

    // Enable Depth Testing. Prevents occluded triangles from being drawn.
    glEnable(GL_DEPTH_TEST);

    // Wireframe mode
    if (m_enableWireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        // Enable default fill mode.
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Index Buffer is already bound, and so the pointer to the IB location can just be nullptr.
    glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

    // Not using Unbind()s at the moment, unnecessary for OpenGL.
    // Normally just a waste of performance.
}

void Renderer::EnableWireFrameMode(bool enable)
{
    m_enableWireFrameMode = enable;
}
