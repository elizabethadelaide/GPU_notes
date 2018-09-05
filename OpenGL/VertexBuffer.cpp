#include "VertexBuffer.h"

#include "Renderer.h"

//Cosntructor
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	//Pass in pointer to unsigned to buffer
	GLCall(glGenBuffers(1, &m_RendererID));
	//Bind buffer array by ID
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	//Type, Size, data, usage
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

//Desctrctor
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

//Bind id
void VertexBuffer::Bind() const{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

//Unbind (set ID to 0)
void VertexBuffer::Unbind() const{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}