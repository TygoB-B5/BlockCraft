#include "world.h"

namespace blockgame
{

	blockVertex sideConstants::bottom[4] =
	{
		blockVertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		blockVertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
	};

	blockVertex sideConstants::top[4] =
	{
		blockVertex(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
		blockVertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
	};

	blockVertex sideConstants::left[4] =
	{
		blockVertex(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
		blockVertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		blockVertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
		blockVertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
	};

	blockVertex sideConstants::right[4] =
	{
		blockVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		blockVertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
	};

	blockVertex sideConstants::front[4] =
	{
		blockVertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
		blockVertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		blockVertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)),
	};

	blockVertex sideConstants::back[4] =
	{
		blockVertex(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)),
		blockVertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)),
		blockVertex(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)),
		blockVertex(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)),
	};

	const char* world::_vertexShaderSrc = R"(

		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 uTexCoord;
		uniform mat4 uViewMatrix;
		uniform mat4 uModelMatrix;

		void main()
		{
			vec4 pos = uViewMatrix * uModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			gl_Position = pos;
			uTexCoord = aTexCoord;
		}

		)";



	const char* world::_fragmentShaderSrc = R"(

		#version 330 core
		out vec4 FragColor;

		in vec2 uTexCoord;
		uniform sampler2D uTexture;

		void main()
		{
			FragColor = texture(uTexture, uTexCoord);
		}

)";
}