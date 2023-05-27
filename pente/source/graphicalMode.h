#pragma once
#include"texture.h"
#include"shader.h"
#include"indexBuffer.h"
#include"vertexBuffer.h"
#include"vertexArray.h"
#include"vertexBufferLayout.h"
#include"vendor/glm/glm.hpp"
#include<GLFW/glfw3.h>
#include"Renderer.h"
#include"declarations.h"


typedef struct {
	int x;
	int y;
	int colour;
} pieceToDraw;

class graphicalInterface {
private:
	float squareVertex[16];
	unsigned int squareIndices[6];
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
	bool shouldCloseWindow();
};

coordinates fetchPosition();