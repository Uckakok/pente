#pragma once

#include<GL/glew.h>
#include"vertexArray.h"
#include"indexBuffer.h"
#include"shader.h"

#define GLAD_GET_ERROR_MAX 25
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void clear() const;
	void draw(const vertexArray& va, const indexBuffer& ib, const Shader& shader) const;
};