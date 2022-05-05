#pragma once

#include "block/blocktexturelibrary.h"
#include "block/block.h"
#include "world.h"
#include "glm/glm.hpp"
#include <vector>

#define CHUNK_HEIGHT 16
#define CHUNK_SIZE 16


namespace blockcraft
{
	class world;

	class chunk
	{
	public:

		chunk(const glm::vec2& cord, world* world);

		void init();

	private:

		inline void generateChunkData();

		inline void constructRenderingData();

		inline void updateVisibilityDataForBlock(uint8_t x, uint8_t y, uint8_t z);

		inline uint8_t getBlockIdFromDifferentChunk(const glm::vec2& chunkPosition, uint8_t x, uint8_t y, uint8_t z);

		inline void calculateAllBlockVisibility();

		inline void calculateEdgeBlockVisibility();

	public:

		void updateSurroundingChunks();

		std::pair<float*, uint32_t> getChunkVertexData();

		std::pair<uint32_t*, uint32_t> getChunkIndexData();

		glm::mat4 getModelMatrix() const;

		void setBlock(uint8_t x, uint8_t y, uint8_t z, uint8_t id);

		bool getHasNewVertexData() const { return _hasNewVertexData; }
		bool getHasNewIndexData() const { return _hasNewIndexData; }

		glm::vec2 getChunkPosition() const { return _chunkPosition; }

 		inline uint8_t getBlockDataAtPosition(uint8_t x, uint8_t y, uint8_t z) const { return _blockData[x][y][z]; }

	private:

		// Renderer data.
		bool _hasNewIndexData, _hasNewVertexData;

		blockVertex _blockVertices[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		uint32_t _blockIndices[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE * 6];

		uint32_t _blockVerticesSize;
		uint32_t _blockIndicesSize;



		// Block data and block visibility.
		uint8_t _blockData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
		bool _visibleSides[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE][6];

		
		// Chunk data
		glm::vec2 _chunkPosition;
		chunk* _cachedChunk;

		// Ptr to base world used to interact with other chunks.
		world* _world;

		// Noise ptr used for world generation.
		glr::perlinnoise1d* _noise;



	};

}