#include "vertex_array.h"
#include "renderer.h"
#include "vertex_buffer_layout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_id));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::AddBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout)
{
	Bind();
	buffer.Bind();

	const auto &elements = layout.GetElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto &element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));

		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_id));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
