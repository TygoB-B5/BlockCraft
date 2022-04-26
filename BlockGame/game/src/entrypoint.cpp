#include <iostream>
#include "engine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


// Shader sources

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Basic fragment shader
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";




int main()
{
	engine::renderer r(engine::window(100, 100, "window"), engine::rendererSettings());

	float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
	};


	r.init();

	// Generate vertex buffer object (VBO) and vertex array object (VAO)
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO, then bind VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Copy the vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set attributes that describe how OpenGL should interpret the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Unbind so other calls won't modify VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	r.setShader(vertexShaderSource, fragmentShaderSource);

	while (!glfwWindowShouldClose(r._window.get())) {

		r.clear();
		r.draw();

		glBindVertexArray(VAO);

		r._window.swapBuffer();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}