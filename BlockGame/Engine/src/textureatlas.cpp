#include "textureatlas.h"

#include <utility>

namespace glr
{

	textureAtlas::textureAtlas(texture* texture, uint32_t spriteSize)
		: _normalizedSize((float)spriteSize / (float)texture->getWidth())
	{
	}

	std::pair<float, float> textureAtlas::getBottomLeftTexCoordFromId(uint32_t row, uint32_t colum)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.second += row * _normalizedSize;
		cords.first += colum * _normalizedSize;

		assert(!(cords.first > 1.0f), "Row out of range");
		assert(!(cords.second > 1.0f), "Colum out of range");

		return cords;
	}

	std::pair<float, float> textureAtlas::getBottomRightTexCoordFromId(uint32_t row, uint32_t colum)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.second += row * _normalizedSize;
		cords.first += (colum * _normalizedSize) + _normalizedSize;

		assert(!(cords.first > 1.0f), "Row out of range");
		assert(!(cords.second > 1.0f), "Colum out of range");

		return cords;
	}

	std::pair<float, float> textureAtlas::getTopLeftTexCoordFromId(uint32_t row, uint32_t colum)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.second += (row * _normalizedSize) + _normalizedSize;
		cords.first += colum * _normalizedSize;

		assert(!(cords.first > 1.0f), "Row out of range");
		assert(!(cords.second > 1.0f), "Colum out of range");

		return cords;
	}

	std::pair<float, float> textureAtlas::getTopRightTexCoordFromId(uint32_t row, uint32_t colum)
	{
		std::pair<float, float> cords = { 0.0f, 0.0f };

		cords.second += (row * _normalizedSize) + _normalizedSize;
		cords.first += (colum * _normalizedSize) + _normalizedSize;

		assert(!(cords.first > 1.0f), "Row out of range");
		assert(!(cords.second > 1.0f), "Colum out of range");

		return cords;
	}



}

