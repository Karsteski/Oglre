#pragma once

#include <cstdint>
#include <vector>
namespace Oglre {
// Handles creation, binding, data storage and deletion of an Index Buffer Object.
class IndexBuffer {

public:
    IndexBuffer(const std::vector<uint32_t> data);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline uint32_t GetCount() const
    {
        return m_Count;
    }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};
}