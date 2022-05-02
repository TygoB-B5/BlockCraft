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

	std::pair<float, float> textureAtlas::getBottomLeftTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % _textureWidth / _pixelSize;
		uint32_t y = id / _textureWidth / _pixelSize;
		return getBottomLeftTexCoordFromId(x, y);
	}

	std::pair<float, float> textureAtlas::getBottomRightTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % _textureWidth / _pixelSize;
		uint32_t y = id / _textureWidth / _pixelSize;
		return getBottomRightTexCoordFromId(x, y);
	}

	std::pair<float, float> textureAtlas::getTopLeftTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % _textureWidth / _pixelSize;
		uint32_t y = id / _textureWidth / _pixelSize;
		return getTopLeftTexCoordFromId(x, y);
	}

	std::pair<float, float> textureAtlas::getTopRightTexCoordFromId(uint32_t id)
	{
		uint32_t x = id % _textureWidth / _pixelSize;
		uint32_t y = id / _textureWidth / _pixelSize;
		return getTopRightTexCoordFromId(x, y);
	}


	std::pair<float, float> textureAtlas::getBottomLeftTexCoordFromId(uint32_t x, uint32_t y)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.first += x * _normalizedSize;
		cords.second += y * _normalizedSize;

		assert(!(cords.first > 1.0f), "x out of range");
		assert(!(cords.second > 1.0f), "y out of range");

		return cords;
	}

	std::pair<float, float> textureAtlas::getBottomRightTexCoordFromId(uint32_t x, uint32_t y)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.first += x * _normalizedSize;
		cords.second += (y * _normalizedSize) + _normalizedSize;

		assert(!(cords.first > 1.0f), "x out of range");
		assert(!(cords.second > 1.0f), "y out of range");

		return cords;
	}

	std::pair<float, float> textureAtlas::getTopLeftTexCoordFromId(uint32_t x, uint32_t y)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.first += (x * _normalizedSize) + _normalizedSize;
		cords.second += y * _normalizedSize;

		assert(!(cords.first > 1.0f), "x out of range");
		assert(!(cords.second > 1.0f), "y out of range");

		return cords;
	}

	std::pair<float, float> textureAtlas::getTopRightTexCoordFromId(uint32_t x, uint32_t y)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.first += (x * _normalizedSize) + _normalizedSize;
		cords.second += (y * _normalizedSize) + _normalizedSize;

		assert(!(cords.first > 1.0f), "x out of range");
		assert(!(cords.second > 1.0f), "y out of range");

		return cords;
	}



}

