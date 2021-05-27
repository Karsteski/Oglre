#include "Renderer.h"

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ibo, const Shader& shader)
{
    shader.Bind();
    va.Bind();
    ibo.Bind();

    // Index Buffer is already bound, and so the pointer to the IB location can just be nullptr.
    glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

    // Not using Unbind()s at the moment, unnecessary for OpenGL.
    // Normally just a waste of performance.
}