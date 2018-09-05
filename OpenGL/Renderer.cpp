#include "Renderer.h"

#include <iostream>


void GLClearError() {
	//clear errors in gl error
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line) {
	if (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << ")"
			<< function << ", " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
	/*Do all the binds*/
	shader.Bind();
	//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
	va.Bind();
	ib.Bind();
	//Be careful of type later...
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const{
	glClear(GL_COLOR_BUFFER_BIT);
}