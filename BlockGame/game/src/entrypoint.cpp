#include "engine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "rendering/spectatorcameracontroller.h"

const char* vertexShaderSource = R"(

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 uTexCoord;
uniform mat4 uViewMatrix;

void main()
{
	vec4 pos = uViewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = pos;
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



std::vector<uint32_t> indices
{
	0, 1, 2, 2, 1, 3,
	4, 5, 6, 6, 5, 7,

	1, 4, 3, 3, 4, 6,
	5, 0, 7, 7, 0, 2,

	5, 4, 0, 0, 4, 1,
	2, 3, 7, 7, 3, 6
};



int main()
{
	auto window = glr::window(1000, 1000, "My window", true);

	auto settings = glr::renderer::rendererSettings();
	settings.CullingMode = glr::renderer::rendererSettings::cullingMode::Back;
	settings.ClearColor = { 1.0f, 0.0f, 0.5f, 1.0f };

	glr::renderer renderer(&window, settings);
	glr::input input(&window);

	glr::texture texture("minecraft.jpg");

	glr::textureAtlas atlas(&texture, 16);

	int row = 15;
	int colum = 4;

	std::vector<float> vertices
	{
		-0.5f, -0.5f, 0.0f,  atlas.getBottomLeftTexCoordFromId(row, colum).first, atlas.getBottomLeftTexCoordFromId(row, colum).second,
		 0.5f, -0.5f, 0.0f,  atlas.getBottomRightTexCoordFromId(row, colum).first, atlas.getBottomRightTexCoordFromId(row, colum).second,
		-0.5f,  0.5f, 0.0f,  atlas.getTopLeftTexCoordFromId(row, colum).first, atlas.getTopLeftTexCoordFromId(row, colum).second,
		 0.5f,  0.5f, 0.0f,  atlas.getTopRightTexCoordFromId(row, colum).first, atlas.getTopRightTexCoordFromId(row, colum).second,

		 0.5f, -0.5f, -1.0f,  atlas.getBottomLeftTexCoordFromId(row, colum).first, atlas.getBottomLeftTexCoordFromId(row, colum).second,
		-0.5f, -0.5f, -1.0f,  atlas.getBottomRightTexCoordFromId(row, colum).first, atlas.getBottomRightTexCoordFromId(row, colum).second,
		 0.5f,  0.5f, -1.0f,  atlas.getTopLeftTexCoordFromId(row, colum).first, atlas.getTopLeftTexCoordFromId(row, colum).second,
		-0.5f,  0.5f, -1.0f,  atlas.getTopRightTexCoordFromId(row, colum).first, atlas.getTopRightTexCoordFromId(row, colum).second,
	};


	glr::vertexBuffer vertBuffer(&vertices[0], vertices.size());

	glr::shader shader(vertexShaderSource, fragmentShaderSource);


	glr::elementBuffer elemBuffer(&indices[0], indices.size());


	glr::vertexLayout<2> s;
	s.Amount[0] = 3;
	s.Amount[1] = 2;
	s.DataType[0] = GL_FLOAT;
	s.DataType[1] = GL_FLOAT;
	s.Stride = 5 * sizeof(float);

	s.bind();


	vertBuffer.unbind();
	shader.unbind();
	elemBuffer.unbind();

	blockgame::spectatorCameraController cont(&input, 100, 1 / 1, 0.001f, 10000);

	while (!glfwWindowShouldClose(renderer.getWindow()->getGlfwWindow()))
	{
		renderer.clear();
		texture.bind(0);


		glm::mat4 m = cont.getCamera().getViewProjectionMatrix();
		shader.setUniformMat4("uViewMatrix", &m[0][0]);

		cont.update(0.01f);

		if (input.isScrollingDown())
			std::cout << "";

		if (input.isScrollingUp())
			std::cout << "";

		shader.bind();
		vertBuffer.bind();
		elemBuffer.bind();
		input.clear();
		renderer.draw(elemBuffer.getElementAmount());

		window.update();
	}

	glfwTerminate();
	return 0;
}