#include "chunk.h"
#include "glm/gtc/matrix_transform.hpp"


namespace blockcraft
{

	chunk::chunk(const glm::vec2& cord, world* world)
		: _chunkPosition(cord), _world(world), _hasNewVertexData(false), _hasNewIndexData(false), _cachedChunk(this)
	{}

	void chunk::init()
	{

		// Construct new chunk data.
		generateChunkData();

		calculateAllBlockVisibility();

		constructRenderingData();

		// Recalculate all visible sides for surrounding chunks.
		updateSurroundingChunks();
	}

	void chunk::generateChunkData()
	{

		// Block Layers
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (size_t z = 0; z < CHUNK_SIZE; z++)
				{
						_blockData[x][y][z] = ID_BLOCK_AIR;
				}
			}
		}

		

		glr::perlinnoise2d* noise = _world->getNoise();

		for (size_t x = 0; x < 16; x++)
		{
			for (size_t z = 0; z < 16; z++)
			{
				int h = noise->getHeightAtPosition(x + (CHUNK_SIZE * _chunkPosition.x), z + (CHUNK_SIZE * _chunkPosition.y)) * CHUNK_HEIGHT - 1;
				_blockData[x][h][z] = ID_BLOCK_GRASS;

				for (size_t i = 0; i < h; i++)
				{
					_blockData[x][i][z] = ID_BLOCK_STONE;
				}


				for (int i = h - 4; i < h; i++)
				{
					_blockData[x][i < 0 ? 0 : i][z] = ID_BLOCK_DIRT;
				}
			}
		}
		
	}

	void chunk::calculateAllBlockVisibility()
	{
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (size_t z = 0; z < CHUNK_SIZE; z++)
				{
					updateVisibilityDataForBlock(x, y, z);
				}
			}
		}
	}

	void chunk::calculateEdgeBlockVisibility()
	{

		// Loop through all the blocks on the edge of the chunk.
		
		for (size_t xz = 0; xz < CHUNK_SIZE; xz++)
		{
			for (size_t y = 0; y < CHUNK_HEIGHT; y++)
			{
				updateVisibilityDataForBlock(0, y, xz);
				updateVisibilityDataForBlock(CHUNK_SIZE - 1, y, xz);

				updateVisibilityDataForBlock(xz, y, 0);
				updateVisibilityDataForBlock(xz, y, CHUNK_SIZE - 1);
			}
		}
	}

	void chunk::constructRenderingData()
	{

		// Reset vertex and index buffer data.
		memset(&_blockVertices[0], 0, sizeof(_blockVertices));
		memset(&_blockIndices[0], 0, sizeof(_blockIndices));


		// Enable has new data.
		_hasNewVertexData = true;
		_hasNewIndexData = true;


		uint32_t i = 0;
		uint32_t vertSize = 0;
		uint32_t elemSize = 0;
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (size_t z = 0; z < CHUNK_SIZE; z++)
				{

					// Skip loop if block is air.
					if (_blockData[x][y][z] == ID_BLOCK_AIR)
						continue;


					// Add sides that need to be rendered.
					for (size_t s = 0; s < 6; s++)
					{

						// Skip rendering if the side is not visible.
						if (!_visibleSides[z][y][x][s])
							continue;

						typedef blockSides sc;


						// Get required texture cords for current side.
						std::array<glm::vec2, 4> tex = _world->getBlockLibrary()->getTextureCoordSideFromId(_blockData[x][y][z], s);


						// Select which side to add based on index vertSize.
						switch (s)
						{
						case 0: // Bottom
							_blockVertices[vertSize]     = (blockVertex({sc::bottom[0].x + x, sc::bottom[0].y + y, sc::bottom[0].z + z}, tex[0]));
							_blockVertices[vertSize + 1] = (blockVertex({ sc::bottom[1].x + x, sc::bottom[1].y + y, sc::bottom[1].z + z }, tex[1]));
							_blockVertices[vertSize + 2] = (blockVertex({ sc::bottom[2].x + x, sc::bottom[2].y + y, sc::bottom[2].z + z }, tex[2]));
							_blockVertices[vertSize + 3] = (blockVertex({ sc::bottom[3].x + x, sc::bottom[3].y + y, sc::bottom[3].z + z }, tex[3]));
							break;

						case 1: // Top
							_blockVertices[vertSize]     = (blockVertex({ sc::top[0].x + x, sc::top[0].y + y, sc::top[0].z + z }, tex[0]));
							_blockVertices[vertSize + 1] = (blockVertex({ sc::top[1].x + x, sc::top[1].y + y, sc::top[1].z + z }, tex[1]));
							_blockVertices[vertSize + 2] = (blockVertex({ sc::top[2].x + x, sc::top[2].y + y, sc::top[2].z + z }, tex[2]));
							_blockVertices[vertSize + 3] = (blockVertex({ sc::top[3].x + x, sc::top[3].y + y, sc::top[3].z + z }, tex[3]));
							break;

						case 2: // Left
							_blockVertices[vertSize]     = (blockVertex({ sc::left[0].x + x, sc::left[0].y + y, sc::left[0].z + z }, tex[1]));
							_blockVertices[vertSize + 1] = (blockVertex({ sc::left[1].x + x, sc::left[1].y + y, sc::left[1].z + z }, tex[2]));
							_blockVertices[vertSize + 2] = (blockVertex({ sc::left[2].x + x, sc::left[2].y + y, sc::left[2].z + z }, tex[3]));
							_blockVertices[vertSize + 3] = (blockVertex({ sc::left[3].x + x, sc::left[3].y + y, sc::left[3].z + z }, tex[0]));
							break;

						case 3: // Right
							_blockVertices[vertSize]     = (blockVertex({ sc::right[0].x + x, sc::right[0].y + y, sc::right[0].z + z }, tex[1]));
							_blockVertices[vertSize + 1] = (blockVertex({ sc::right[1].x + x, sc::right[1].y + y, sc::right[1].z + z }, tex[2]));
							_blockVertices[vertSize + 2] = (blockVertex({ sc::right[2].x + x, sc::right[2].y + y, sc::right[2].z + z }, tex[3]));
							_blockVertices[vertSize + 3] = (blockVertex({ sc::right[3].x + x, sc::right[3].y + y, sc::right[3].z + z }, tex[0]));
							break;

						case 4: // Front
							_blockVertices[vertSize]     = (blockVertex({ sc::front[0].x + x, sc::front[0].y + y, sc::front[0].z + z }, tex[0]));
							_blockVertices[vertSize + 1] = (blockVertex({ sc::front[1].x + x, sc::front[1].y + y, sc::front[1].z + z }, tex[1]));
							_blockVertices[vertSize + 2] = (blockVertex({ sc::front[2].x + x, sc::front[2].y + y, sc::front[2].z + z }, tex[2]));
							_blockVertices[vertSize + 3] = (blockVertex({ sc::front[3].x + x, sc::front[3].y + y, sc::front[3].z + z }, tex[3]));
							break;

						case 5: // Back
							_blockVertices[vertSize]     = (blockVertex({ sc::back[0].x + x, sc::back[0].y + y, sc::back[0].z + z }, tex[2]));
							_blockVertices[vertSize + 1] = (blockVertex({ sc::back[1].x + x, sc::back[1].y + y, sc::back[1].z + z }, tex[3]));
							_blockVertices[vertSize + 2] = (blockVertex({ sc::back[2].x + x, sc::back[2].y + y, sc::back[2].z + z }, tex[0]));
							_blockVertices[vertSize + 3] = (blockVertex({ sc::back[3].x + x, sc::back[3].y + y, sc::back[3].z + z }, tex[1]));
							break;
						}

						vertSize += 4;

						// Update index buffer and add indices for 1 side.
						_blockIndices[elemSize]     = (i);
						_blockIndices[elemSize + 1] = (i + 1);
						_blockIndices[elemSize + 2] = (i + 2);
						_blockIndices[elemSize + 3] = (i + 2);
						_blockIndices[elemSize + 4] = (i + 3);
						_blockIndices[elemSize + 5] = (i);

						elemSize += 6;
						i += 4;
					}
				}
			}

			_blockVerticesSize = vertSize;
			_blockIndicesSize = elemSize;
		}
	}

	uint32_t chunk::getBlockIdFromDifferentChunk(const glm::vec2& chunkPosition, uint32_t x, uint32_t y, uint32_t z)
	{
	
		// Get chunk from position.
		chunk* c = _world->getChunkFromPosition(chunkPosition);;


		// If chunk is found return id from the specified position.
		if (c)
		{
			return c->getBlockDataAtPosition(x, y, z);
		}
			
		return 257;
	}

	void chunk::remove()
	{
		// Write all block data to air blocks.
		memset(&_blockData[0][0][0], ID_BLOCK_AIR, sizeof(_blockData));


		updateSurroundingChunks();
	}

	void chunk::updateSurroundingChunks()
	{
		// Get surrounding chunks data.
		std::array<chunk*, 4> surroundingChunks({
			_world->getChunkFromPosition({ _chunkPosition.x, _chunkPosition.y + 1 }),
			_world->getChunkFromPosition({ _chunkPosition.x, _chunkPosition.y - 1 }),
			_world->getChunkFromPosition({ _chunkPosition.x - 1, _chunkPosition.y }),
			_world->getChunkFromPosition({ _chunkPosition.x + 1, _chunkPosition.y }),
			});

		// If chunk is not nullptr update it.
		for (chunk* c : surroundingChunks)
		{
			if (c)
			{
				c->calculateEdgeBlockVisibility();
				c->constructRenderingData();
			}
		}
	}

	std::pair<float*, uint32_t> chunk::getChunkVertexData()
	{
		_hasNewVertexData = false;
		return { (float*)&_blockVertices[0], _blockVerticesSize * sizeof(blockVertex)};
	}

	std::pair<uint32_t*, uint32_t> chunk::getChunkIndexData()
	{
		_hasNewIndexData = false;
		return { (uint32_t*)&_blockIndices[0], _blockIndicesSize };
	}

	glm::mat4 chunk::getModelMatrix() const
	{
		// Create matrix with a translation to the right position.
		return glm::translate(glm::mat4(1.0f), glm::vec3(_chunkPosition.x * CHUNK_SIZE, 0, _chunkPosition.y * CHUNK_SIZE));
	}

	void chunk::setBlock(uint32_t x, uint32_t y, uint32_t z, uint32_t id)
	{

		// Set right block to id.
		_blockData[x][y][z] = id;


		// Update the blocks surrounding this block.

		if (x > 0)
			updateVisibilityDataForBlock(x - 1, y, z);

		if (y > 0)
			updateVisibilityDataForBlock(x, y - 1, z);

		if (z > 0)
			updateVisibilityDataForBlock(x, y, z - 1);

		if (x < CHUNK_SIZE - 1)
			updateVisibilityDataForBlock(x + 1, y, z);

		if (y < CHUNK_HEIGHT - 1)
			updateVisibilityDataForBlock(x, y + 1, z);

		if (z < CHUNK_SIZE - 1)
			updateVisibilityDataForBlock(x, y, z + 1);
			
		updateVisibilityDataForBlock(x, y, z);

		//TODO Make this more efficient.
		//
		// Now it recalculates every vertex every time you place a block, this is innefficient.
		// Make it so it only updates the vertices around it.
		//

		constructRenderingData();
	}

	void chunk::updateVisibilityDataForBlock(uint32_t x, uint32_t y, uint32_t z)
	{

		// Hide all sides if the block is air.
		if (_blockData[x][y][z] == ID_BLOCK_AIR)
		{
			return;
		}


		// Update all 6 sides

		// Bottom
		if (y > 0)
			_visibleSides[z][y][x][0] = blockData::isTransparent(_blockData[x][y - 1][z]);
		else
			_visibleSides[z][y][x][0] = true;

		// Top
		if (y < CHUNK_HEIGHT - 1)
			_visibleSides[z][y][x][1] = blockData::isTransparent(_blockData[x][y + 1][z]);
		else
			_visibleSides[z][y][x][1] = true;

		// Left
		if (x > 0)
			_visibleSides[z][y][x][2] = blockData::isTransparent(_blockData[x - 1][y][z]);
		else
			_visibleSides[z][y][x][2] = blockData::isTransparent(getBlockIdFromDifferentChunk({ _chunkPosition.x - 1, _chunkPosition.y }, CHUNK_SIZE - 1, y, z));

		// Right
		if (x < CHUNK_SIZE - 1)
			_visibleSides[z][y][x][3] = blockData::isTransparent(_blockData[x + 1][y][z]);
		else
			_visibleSides[z][y][x][3] = blockData::isTransparent(getBlockIdFromDifferentChunk({ _chunkPosition.x + 1, _chunkPosition.y }, 0, y, z));

		// Front
		if (z > 0)
			_visibleSides[z][y][x][4] = blockData::isTransparent(_blockData[x][y][z - 1]);
		else
			_visibleSides[z][y][x][4] = blockData::isTransparent(getBlockIdFromDifferentChunk({ _chunkPosition.x, _chunkPosition.y - 1 }, x, y, CHUNK_SIZE - 1));

		// Back
		if (z < CHUNK_SIZE - 1)
			_visibleSides[z][y][x][5] = blockData::isTransparent(_blockData[x][y][z + 1]);
		else
			_visibleSides[z][y][x][5] = blockData::isTransparent(getBlockIdFromDifferentChunk({ _chunkPosition.x, _chunkPosition.y + 1 }, x, y, 0));

	}

}