#pragma once

#include "block/blocktexturelibrary.h"
#include "block/block.h"
#include "world.h"
#include "glm/glm.hpp"
#include <vector>

#define CHUNK_HEIGHT 128
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

		inline void updateVisibilityDataForBlock(uint32_t x, uint32_t y, uint32_t z);

		inline uint32_t getBlockIdFromDifferentChunk(const glm::vec2& chunkPosition, uint32_t x, uint32_t y, uint32_t z);

		inline void calculateAllBlockVisibility();

		inline void calculateEdgeBlockVisibility();

	public:

		void remove();

		void updateSurroundingChunks();

		std::pair<float*, uint32_t> getChunkVertexData();

		std::pair<uint32_t*, uint32_t> getChunkIndexData();

		glm::mat4 getModelMatrix() const;

		void setBlock(uint32_t x, uint32_t y, uint32_t z, uint32_t id);

		bool getHasNewVertexData() const { return _hasNewVertexData; }
		bool getHasNewIndexData() const { return _hasNewIndexData; }

		glm::vec2 getChunkPosition() const { return _chunkPosition; }

		inline uint32_t getBlockDataAtPosition(uint32_t x, uint32_t y, uint32_t z) const { return _blockData[x][y][z]; }

	private:

		// Renderer data.
		bool _hasNewIndexData, _hasNewVertexData;

		blockVertex _blockVertices[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		uint32_t _blockIndices[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE * 6];

		uint32_t _blockVerticesSize;
		uint32_t _blockIndicesSize;



		// Block data and block visibility.
		uint32_t _blockData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
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