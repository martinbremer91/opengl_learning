#pragma once

#include <vector>
#include <iostream>
#include "glad.h"
#include "renderer.h"

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:          return sizeof(GLfloat);
            case GL_UNSIGNED_INT:   return sizeof(GLuint);
            case GL_UNSIGNED_BYTE:  return sizeof(GLubyte);
        }

        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
public:
    VertexBufferLayout() 
        : m_stride(0) {}

    template<typename T>
    void Push(unsigned int count);

    inline const std::vector<VertexBufferElement> GetElements() const& { return m_elements; }
    inline unsigned int GetStride() const { return m_stride; }
};
