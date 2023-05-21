#include"Renderer.h"
#include<iostream>

void GLClearError() {
	for (int i = 0; i < GLAD_GET_ERROR_MAX; ++i) {
		while (glGetError() == GL_NO_ERROR)
			break;;
	}
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[Blad OpenGL] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void renderer::clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void renderer::draw(const vertexArray& va, const indexBuffer& ib, const Shader& shader) const {
	shader.bind();

	va.bind();
	ib.bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}