#pragma once

#include "block.h"
#include "world.h"
#include "glm/glm.hpp"
#include <vector>

#define CHUNK_HEIGHT 32
#define CHUNK_SIZE 16


namespace blockcraft
{
	class world;

	class chunk
	{
	public:

		chunk(const glm::vec2& cord, world* world)
			: _chunkPosition(cord), _world(world), _hasNewVertexData(true), _hasNewIndexData(true)
		{
			constructChunkData();
			calculateAllBlockVisibility();
			constructRendering();
		}


	private:

		void constructChunkData();

		void calculateAllBlockVisibility();

		void updateVisibilityDataForBlock(uint32_t x, uint32_t y, uint32_t z);

		void constructRendering();


	public:

		std::pair<float*, uint32_t> getChunkVertexData();

		std::pair<uint32_t*, uint32_t> getChunkIndexData();

		glm::mat4 getModelMatrix() const;

		void setBlock(uint32_t x, uint32_t y, uint32_t z, uint32_t id);

		bool getHasNewVertexData() const { return _hasNewVertexData; }
		bool getHasNewIndexData() const { return _hasNewIndexData; }

	private:

		bool _hasNewIndexData, _hasNewVertexData;

		glm::vec2 _chunkPosition;
		world* _world;

		std::vector<blockVertex> _blockVertices;
		std::vector<uint32_t> _blockIndices;

		bool _visibleSides[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE][6];
		uint32_t _blockData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
	};

}