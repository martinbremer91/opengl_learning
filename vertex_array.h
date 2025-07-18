#pragma once

#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray
{
private:
    unsigned int m_id;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout);

    void Bind() const;
    void Unbind() const;
};
