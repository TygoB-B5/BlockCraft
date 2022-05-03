#pragma once
#include "block.h"
#include "glm/glm.hpp"
#include <vector>

#define CHUNK_HEIGHT 32
#define CHUNK_SIZE 16


namespace blockcraft
{

	class chunk
	{
	public:

		chunk(const glm::vec2& cord, blockTextureLibrary* blockTextureLib)
			: _chunkPosition(cord), _blockTextureLibrary(blockTextureLib)
		{
			constructChunkData();
			calculateAllBlockVisibility();
			constructRendering();
		}

		void constructChunkData();

		void calculateAllBlockVisibility();

		void constructRendering();

		std::pair<float*, uint32_t> getChunkvertexData();

		std::pair<uint32_t*, uint32_t> getChunkIndexData();

		glm::mat4 getModelMatrix() const;

		inline void setBlockData(uint32_t x, uint32_t y, uint32_t z, uint32_t value);

		inline void updateVisibilityDataForBlock(uint32_t x, uint32_t y, uint32_t z);


	private:

		glm::vec2 _chunkPosition;
		blockTextureLibrary* _blockTextureLibrary;

		std::vector<blockVertex> _blockVertices;
		std::vector<uint32_t> _blockIndices;

		bool _visibleSides[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE][6];
		uint32_t _blockData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
	};


}