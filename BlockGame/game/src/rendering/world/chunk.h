#pragma once

#include "world.h"
#include "block/blocktlibrary.h"
#include "block/block.h"

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
		inline void calculateAllBlockVisibility();
		inline void calculateEdgeBlockVisibility();
		inline void calculateSurroundingBlockVisibility(uint8_t x, uint8_t y, uint8_t z);
		inline void updateSurroundingChunks();

	public:


		// Getters

		// Returns the chunk's raw vertex buffer data.
		std::pair<float*, uint32_t> getChunkVertexData();

		// Returns the chunk's raw index buffer data.
		std::pair<uint32_t*, uint32_t> getChunkIndexData();

		// Returns if the chunk has new vertex data.
		bool getHasNewVertexData() const { return _hasNewVertexData; }

		// Returns if the chunk has new index data.
		bool getHasNewIndexData() const { return _hasNewIndexData; }

		// Returns the location of the chunk in a matrix.
		glm::mat4 getModelMatrix() const;

		// Returns the global of the chunk.
		glm::vec2 getChunkPosition() const { return _chunkPosition; }

		// Returns the Block ID from  the chunk's coordinate system. ( 0, 0, 0 to Chunksize, ChunkHeight, ChunkSize )
		uint8_t getBlock(uint8_t x, uint8_t y, uint8_t z) const { return _blockData[x][y][z]; }

		// Replaces the block from withing the chunk's coordinate system. ( 0, 0, 0 to Chunksize, ChunkHeight, ChunkSize )
		void setBlock(uint8_t x, uint8_t y, uint8_t z, uint8_t id);

	private:

		// Renderer data.
		bool _hasNewIndexData, _hasNewVertexData;

		blockVertex _blockVertices[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE];
		uint32_t _blockElements[CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE * 6];

		uint32_t _blockVerticesSize;
		uint32_t _blockElementsSize;


		// Block data and block visibility.
		uint8_t _blockData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
		bool _visibleSides[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE][6];

		
		// Chunk data
		glm::vec2 _chunkPosition;


		// Ptr to base world used to interact with other chunks.
		world* _world;


		// Noise ptr used for world generation.
		glr::perlinnoise1d* _noise;

	};

}