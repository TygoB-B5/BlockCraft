#include <iostream>
#include "engine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

const char* vertexShaderSource = R"(

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 uTexCoord;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	uTexCoord = aTexCoord;
}

)";

// Basic fragment shader
const char* fragmentShaderSource = R"(

#version 330 core
out vec4 FragColor;

in vec2 uTexCoord;
uniform sampler2D uTexture;

void main()
{
	FragColor = texture(uTexture, uTexCoord);
}

)";




int main()
{
	engine::renderer r(engine::window(1000, 1000, "My window"), engine::rendererSettings());

	float vertices[4 * 5] = {
	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,	
	 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
	};

	uint32_t indices[3 * 2]
	{
		0, 1, 2, 2, 1, 3
	};


	r.init();

	// Generate vertex buffer object (VBO) and vertex array object (VAO)
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	// Bind VAO, then bind VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);


	// Copy the vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glBindVertexArray(VAO);
	
	engine::renderer::shader shader;
	shader.compileShader(vertexShaderSource, fragmentShaderSource);
	shader.bindShader();

	engine::renderer::vertexLayout<2> s;
	s.Amount[0] = 3;
	s.Amount[1] = 2;

	s.DataType[0] = GL_FLOAT;
	s.DataType[1] = GL_FLOAT;

	s.Stride = 5 * sizeof(float);


	r.setVertexLayout(s);

	r.setTexture(0, "test.jpg");

	// Unbind so other calls won't modify VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(r._window.get())) {

		r.clear();

		shader.bindShader();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		r.draw();

		r._window.swapBuffer();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}