#pragma once

#include "renderer.h"


namespace glr
{

	class textureAtlas
	{
	public:
		textureAtlas(texture* texture, uint32_t spriteSize);

		void set(texture* texture, uint32_t spriteSize);

		std::pair<float, float> getBottomLeftTexCoordFromId(uint32_t id);
		std::pair<float, float> getBottomRightTexCoordFromId(uint32_t id);
		std::pair<float, float> getTopLeftTexCoordFromId(uint32_t id);
		std::pair<float, float> getTopRightTexCoordFromId(uint32_t id);

		std::pair<float, float> getBottomLeftTexCoordFromId(uint32_t row, uint32_t colum);
		std::pair<float, float> getBottomRightTexCoordFromId(uint32_t row, uint32_t colum);
		std::pair<float, float> getTopLeftTexCoordFromId(uint32_t row, uint32_t colum);
		std::pair<float, float> getTopRightTexCoordFromId(uint32_t row, uint32_t colum);

	private:

		uint32_t _textureWidth, _textureHeight;
		uint32_t _pixelSize;
		float _normalizedSize;
	};
}