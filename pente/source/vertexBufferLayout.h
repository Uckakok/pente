#pragma once

#include<vector>
#include<GL/glew.h>
#include"Renderer.h"

// Struktura definiuj�ca element bufora wierzcho�k�w
struct vertexBufferElement {
	unsigned int type;

	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type) {
		switch(type)
		{
		case GL_FLOAT:	return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

// Klasa definiuj�ca uk�ad bufora wierzcho�k�w
class vertexBufferLayout {
private:
	std::vector<vertexBufferElement> m_Elements;
	unsigned int m_Stride; //odleg�o�� pomi�dzy kolejnymi wierzcho�kami
public:
	vertexBufferLayout()
		: m_Stride(0) {}

	void push(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * vertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	inline const std::vector<vertexBufferElement> getElements() const { return m_Elements; }
	inline unsigned int getStride() const { return m_Stride; }
};