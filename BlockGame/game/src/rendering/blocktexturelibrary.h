#pragma once

#include "engine.h"

#define BLOCK_TEXTURE_FILE_NAME "minecraft.jpg"
#define BLOCK_TEXTURE_SIZE 16


///////////////////////////////////
///         Side order.         ///
///////////////////////////////////

// 0. Bottom
// 1. Top
// 2. Left
// 3. Right
// 4. Front
// 5. Back

#define ID_BLOCK_AIR   0
#define ID_BLOCK_DIRT  1
#define ID_BLOCK_GRASS 2
#define ID_BLOCK_STONE 3

const uint16_t SIDES_BLOCK_AIR[6] = { 0, 0, 0, 0, 0, 0 };
const uint16_t SIDES_BLOCK_DIRT[6] = { 2, 2, 2, 2, 2, 2 };
const uint16_t SIDES_BLOCK_GRASS[6] = { 2, 0, 3, 3, 3, 3 };
const uint16_t SIDES_BLOCK_STONE[6] = { 1, 1, 1, 1, 1, 1 };


namespace blockcraft
{

	class blockTextureLibrary
	{
	public:

		blockTextureLibrary()
			: _texture(BLOCK_TEXTURE_FILE_NAME), _atlas(&_texture, BLOCK_TEXTURE_SIZE)
		{}

		inline std::array<glm::vec2, 4> getTextureCoordSideFromId(uint32_t id, uint32_t side)
		{

			// Return texCoords from id and side.
			switch (id)
			{
			case ID_BLOCK_AIR: return _atlas.getTexCoord(SIDES_BLOCK_AIR[side]);
			case ID_BLOCK_DIRT: return _atlas.getTexCoord(SIDES_BLOCK_DIRT[side]);
			case ID_BLOCK_GRASS: return _atlas.getTexCoord(SIDES_BLOCK_GRASS[side]);
			case ID_BLOCK_STONE: return _atlas.getTexCoord(SIDES_BLOCK_STONE[side]);
			}

		}

		const glr::texture& getTexture() const { return _texture; }


	private:

		glr::texture _texture;
		glr::textureAtlas _atlas;
	};

}