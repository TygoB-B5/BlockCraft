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
	auto window = sr::window(1000, 1000, "My window", true);

	sr::renderer renderer(window, sr::renderer::rendererSettings({1.0f, 0.5f, 0.0f, 1.0f}));
	sr::input input(window);

	sr::renderer::vertexBuffer vertBuffer(&vertices[0], 20);

	sr::renderer::shader shader(vertexShaderSource, fragmentShaderSource);

	sr::renderer::texture texture("test.jpg");
	sr::renderer::texture texture2("test2.jpg");

	sr::renderer::elementBuffer elemBuffer(&indices[0], 6);


	sr::renderer::vertexLayout<2> s;
	s.Amount[0] = 3;
	s.Amount[1] = 2;
	s.DataType[0] = GL_FLOAT;
	s.DataType[1] = GL_FLOAT;
	s.Stride = 5 * sizeof(float);

	s.bind();


	vertBuffer.unbind();
	shader.unbind();
	elemBuffer.unbind();

	int i = 0;
	int b = 0;
	while (!glfwWindowShouldClose(renderer.getWindow()->getGlfwWindow()))
	{
		renderer.clear();

		b++;
		if(b % 100 == 0)
			i++;

		if(i % 2 == 0)
		texture.bind();
		else
			texture2.bind();

		shader.bind();
		vertBuffer.bind();
		elemBuffer.bind();
		renderer.draw(elemBuffer.getElementAmount());

		window.update();
	}

	glfwTerminate();
	return 0;
}