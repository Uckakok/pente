#pragma once

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include"Renderer.h"
#include"indexBuffer.h"
#include"vertexBuffer.h"
#include"vertexArray.h"
#include"shader.h"
#include"vertexBufferLayout.h"
#include"texture.h"
#include"vendor/glm/glm.hpp"
#include"vendor/glm/gtc/matrix_transform.hpp"
#include"declarations.h"
#include<vector>
#include<cmath>

typedef struct {
	int x;
	int y;
	int colour;
} pieceToDraw;

class graphicalInterface {
private:
	float squareVertex[16] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
	}; 

	unsigned int squareIndices[6] = {
			0, 1, 2,
			2, 3, 0
	};
	GLFWwindow *window;
	GLuint vao;
	vertexArray *va;
	vertexBuffer *vb;
	indexBuffer *ib;
	vertexBufferLayout layout;
	Shader *shader;
	Texture *textureBoard;
	Texture *whitePiece;
	Texture *blackPiece;
	Renderer renderer;
	glm::vec3 *translationA;
	glm::vec3 *translationB;
	glm::vec3 *scaleA;
	glm::vec3 *scaleB;
	glm::mat4 proj;
	std::vector<pieceToDraw> pieces;
public:
	graphicalInterface();
	void blendEnable() const;
	void prepareVertexArray();
	void prepareVertexBuffer();
	void prepareShaders();
	void unbindStuff() const;
	void setupCallbacks() const;
	void setupMatrices();
	void windowUpdate();
	void closeWindow();
	void resetPosition();
	void newPiecesToDraw(const std::vector<pieceToDraw>& newPieces);
};

coordinates fetchPosition();