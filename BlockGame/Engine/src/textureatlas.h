#pragma once

#include "renderer.h"
#include <array>
#include "glm/glm.hpp"

namespace glr
{

	class textureAtlas
	{
	public:
		textureAtlas(texture* texture, uint32_t spriteSize);

		void set(texture* texture, uint32_t spriteSize);

		std::array<glm::vec2, 4> getTexCoord(uint32_t id);

		glm::vec2 getBottomLeftTexCoordFromId(uint32_t id);
		glm::vec2 getBottomRightTexCoordFromId(uint32_t id);
		glm::vec2 getTopLeftTexCoordFromId(uint32_t id);
		glm::vec2 getTopRightTexCoordFromId(uint32_t id);

		glm::vec2 getBottomLeftTexCoordFromId(uint32_t row, uint32_t colum);
		glm::vec2 getBottomRightTexCoordFromId(uint32_t row, uint32_t colum);
		glm::vec2 getTopLeftTexCoordFromId(uint32_t row, uint32_t colum);
		glm::vec2 getTopRightTexCoordFromId(uint32_t row, uint32_t colum);

	private:

		uint32_t _textureWidth, _textureHeight;
		uint32_t _pixelSize;
		float _normalizedSize;
	};
}