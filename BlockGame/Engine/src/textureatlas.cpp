#include "textureatlas.h"

#include <utility>

namespace glr
{

	textureAtlas::textureAtlas(texture* texture, uint32_t spriteSize)
	{
		set(texture, spriteSize);
	}

	void textureAtlas::set(texture* texture, uint32_t spriteSize)
	{
		_normalizedSize = ((float)spriteSize / (float)texture->getWidth());
		_textureWidth = (texture->getWidth());
		_textureHeight = (texture->getHeight());
		_pixelSize = (spriteSize);
	}

	std::array<glm::vec2, 4> textureAtlas::getTexCoord(uint32_t id)
	{
		return
		{
				getBottomLeftTexCoordFromId(id),
				getBottomRightTexCoordFromId(id),
				getTopRightTexCoordFromId(id),
				getTopLeftTexCoordFromId(id),
		};
	}

	glm::vec2 textureAtlas::getBottomLeftTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % (_textureWidth / _pixelSize);
		uint32_t y = id / _textureWidth / _pixelSize;
		return getBottomLeftTexCoordFromId(x, y);
	}

	glm::vec2 textureAtlas::getBottomRightTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % (_textureWidth / _pixelSize);
		uint32_t y = id / _textureWidth / _pixelSize;
		return getBottomRightTexCoordFromId(x, y);
	}

	glm::vec2 textureAtlas::getTopLeftTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % (_textureWidth / _pixelSize);
		uint32_t y = id / _textureWidth / _pixelSize;
		return getTopLeftTexCoordFromId(x, y);
	}

	glm::vec2 textureAtlas::getTopRightTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % (_textureWidth / _pixelSize);
		uint32_t y = id / _textureWidth / _pixelSize;
		return getTopRightTexCoordFromId(x, y);
	}


	glm::vec2 textureAtlas::getBottomLeftTexCoordFromId(uint32_t x, uint32_t y)
	{
		glm::vec2 cords = { 0.0f, 0.0f };

		cords.x += x * _normalizedSize;
		cords.y += y * _normalizedSize;

		assert(!(cords.x > 1.0f), "x out of range");
		assert(!(cords.y > 1.0f), "y out of range");

		return cords;
	}

	glm::vec2 textureAtlas::getBottomRightTexCoordFromId(uint32_t x, uint32_t y)
	{
		glm::vec2 cords = { 0.0f, 0.0f };

		cords.x += x * _normalizedSize;
		cords.y += (y * _normalizedSize) + _normalizedSize;

		assert(!(cords.x > 1.0f), "x out of range");
		assert(!(cords.y > 1.0f), "y out of range");

		return cords;
	}

	glm::vec2 textureAtlas::getTopLeftTexCoordFromId(uint32_t x, uint32_t y)
	{
		glm::vec2 cords = { 0.0f, 0.0f };

		cords.x += (x * _normalizedSize) + _normalizedSize;
		cords.y += y * _normalizedSize;

		assert(!(cords.x > 1.0f), "x out of range");
		assert(!(cords.y > 1.0f), "y out of range");

		return cords;
	}

	glm::vec2 textureAtlas::getTopRightTexCoordFromId(uint32_t x, uint32_t y)
	{
		glm::vec2 cords = { 0.0f, 0.0f };

		cords.x += (x * _normalizedSize) + _normalizedSize;
		cords.y += (y * _normalizedSize) + _normalizedSize;

		assert(!(cords.x > 1.0f), "x out of range");
		assert(!(cords.y > 1.0f), "y out of range");

		return cords;
	}



}

