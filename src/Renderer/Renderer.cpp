#include "Renderer.h"

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ibo, const Shader& shader)
{
    shader.Bind();
    va.Bind();
    ibo.Bind();

    // Enable Depth Testing. Prevents occluded triangles from being drawn.
    glEnable(GL_DEPTH_TEST);

    // Enable wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Enable default fill mode.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Index Buffer is already bound, and so the pointer to the IB location can just be nullptr.
    glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

    // Not using Unbind()s at the moment, unnecessary for OpenGL.
    // Normally just a waste of performance.
}