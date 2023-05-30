#pragma once
#include"graphicalMode.h"
#include<GL/glew.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include"vendor/glm/gtc/matrix_transform.hpp"
#include<vector>
#include<cmath>


coordinates communication = { -1, -1 };
float disBetweenSquares = 420.0f / 18.0f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		communication.x = (int)round((xpos - 60) / disBetweenSquares);
		communication.y = (int)round((ypos - 60) / disBetweenSquares);
	}
}


graphicalInterface::graphicalInterface() :va(NULL) {

	if (!glfwInit()) {
		std::cout << "Nie mozna otworzyc okna. Krytyczny blad aplikacji" << std::endl;
		__debugbreak();
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(960, 540, "Pente", NULL, NULL);
	if (!window)
	{
		std::cout << "Nie mozna otworzyc okna. Krytyczny blad aplikacji" << std::endl;
		glfwTerminate();
		__debugbreak();
		return;
	}
	GLCall(glfwMakeContextCurrent(window));

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Nie mozna otworzyc okna. Krytyczny blad aplikacji" << std::endl;
		glfwTerminate();
		__debugbreak();
		return;
	}
}

void graphicalInterface::blendEnable() const
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void graphicalInterface::prepareVertexArray()
{
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
}

void graphicalInterface::prepareVertexBuffer() {
	vb = new vertexBuffer(squareVertex, 4 * 4 * sizeof(float));
	layout.push(2);
	layout.push(2);
	va = new vertexArray();
	va->addBuffer(*vb, layout);
	ib = new indexBuffer(squareIndices, 6);
}

void graphicalInterface::prepareShaders()
{
	shader = new Shader("resources/shaders/basic.shader");
	shader->bind();
	whitePiece = new Texture("resources/textures/whitePiece.png");
	whitePiece->bind();
	shader->setUniform1i("u_Texture", 0);
	blackPiece = new Texture("resources/textures/blackPiece.png");
	blackPiece->bind();
	shader->setUniform1i("u_Texture", 0);
	textureBoard = new Texture("resources/textures/board.png");
	textureBoard->bind();
	shader->setUniform1i("u_Texture", 0);
}

void graphicalInterface::unbindStuff() const
{
	GLCall(glBindVertexArray(0));
	va->unbind();
	shader->unbind();
	vb->unbind();
	ib->unbind();
}

void graphicalInterface::setupCallbacks() const
{
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void graphicalInterface::setupMatrices()
{
	translationA = new glm::vec3(270 / 5.2, 270 / 5.2, 0);
	translationB = new glm::vec3(400, 200, 0);
	scaleA = new glm::vec3(5.2, 5.2, 0);
	scaleB = new glm::vec3(0.25f, 0.25f, 0);
	proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
}

void graphicalInterface::windowUpdate()
{
	renderer.clear();

	shader->bind();
	{
		textureBoard->bind();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), (const glm::vec3)*translationA);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), (const glm::vec3)*scaleA);
		glm::mat4 mvp = proj * scale * model;
		shader->setUniformMat4f("u_MVP", mvp);
		renderer.draw(*va, *ib, *shader);
	}
	for (auto& piece : pieces) {
		if (piece.colour == WHITE) whitePiece->bind();
		else if (piece.colour == BLACK) blackPiece->bind();
		else continue;
		float x = piece.x * disBetweenSquares + 60;
		float y = piece.y * disBetweenSquares + 60;
		translationB = new glm::vec3(y * 4, (540 - x) * 4, 0);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), (const glm::vec3)*translationB);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), (const glm::vec3)*scaleB);
		glm::mat4 mvp = proj * scale * model;
		shader->setUniformMat4f("u_MVP", mvp);
		renderer.draw(*va, *ib, *shader);
	}

	GLCall(glfwSwapBuffers(window));
	GLCall(glfwPollEvents());

	if (glfwWindowShouldClose(window)) {
		communication.x = -2;
		communication.y = -2;
	}
}

void graphicalInterface::closeWindow()
{
	communication = { -1, -1 };
	glfwTerminate();
}

void graphicalInterface::resetPosition()
{
	communication = { -1, -1 };
}

void graphicalInterface::newPiecesToDraw(const std::vector<pieceToDraw>& newPieces)
{
	pieces = newPieces;
}

bool graphicalInterface::shouldCloseWindow()
{
	if (glfwWindowShouldClose(window)) {
		return true;
	}
	else {
		return false;
	}
}


coordinates fetchPosition()
{
	return communication;
}