#include "engine.h"


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



std::vector<float> vertices
{
-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
 0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
-0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
};



std::vector<uint32_t> indices
{
	0, 1, 2, 2, 1, 3
};



int main()
{
	engine::renderer renderer(engine::window(1000, 1000, "My window"), engine::rendererSettings());

	renderer.init();

	engine::renderer::vertexBuffer vertBuffer(&vertices[0], 20);

	engine::renderer::shader shader(vertexShaderSource, fragmentShaderSource);

	engine::renderer::texture texture("test.jpg");

	engine::renderer::elementBuffer elemBuffer(&indices[0], 6);


	engine::renderer::vertexLayout<2> s;
	s.Amount[0] = 3;
	s.Amount[1] = 2;
	s.DataType[0] = GL_FLOAT;
	s.DataType[1] = GL_FLOAT;
	s.Stride = 5 * sizeof(float);

	s.bind();


	vertBuffer.unbind();
	shader.unbind();
	elemBuffer.unbind();

	while (!glfwWindowShouldClose(renderer.getWindow()->getGlfwWindow()))
	{
		renderer.clear();
		texture.bind();
		shader.bind();
		vertBuffer.bind();
		elemBuffer.bind();

		renderer.draw(elemBuffer.getElementAmount());

		renderer.getWindow()->swapBuffer();
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}