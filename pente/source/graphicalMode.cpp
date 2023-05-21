#pragma once
#include"graphicalMode.h"
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
#include"vendor/imgui/imgui.h"
#include"vendor/imgui/imgui_impl_glfw_gl3.h"

int testWindow() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	GLCall(glfwMakeContextCurrent(window));

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		GLCall(glfwTerminate());
		return -1;
	}
	{
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,
			 50.0f, -50.0f, 1.0f, 0.0f,
			 50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f,  50.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));


		vertexArray va;
		vertexBuffer vb(positions, 4 * 4 * sizeof(float));
		vertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);
		va.addBuffer(vb, layout);

		indexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		Shader shader("resources/shaders/basic.shader");
		shader.bind();
		shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("resources/textures/board.png");
		texture.bind();
		shader.setUniform1i("u_Texture", 0);

		GLCall(glBindVertexArray(0));

		va.unbind();
		shader.unbind();
		vb.unbind();
		ib.unbind();

		renderer renderer1;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		float r = 0.0f;
		float increment = 0.05f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer1.clear();

			ImGui_ImplGlfwGL3_NewFrame();

			shader.bind();
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader.setUniformMat4f("u_MVP", mvp);
				renderer1.draw(va, ib, shader);
			}
			shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 mvp = proj * view * model;
				shader.setUniformMat4f("u_MVP", mvp);
				renderer1.draw(va, ib, shader);
			}

			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}
			r += increment;

			{
				ImGui::SliderFloat2("Translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat2("Translation B", &translationB.x, 0.0f, 960.0f);
			}


			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			GLCall(glfwPollEvents());
		}
		}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}