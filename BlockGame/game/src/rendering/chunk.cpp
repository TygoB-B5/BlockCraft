#include "chunk.h"
#include "glm/gtc/matrix_transform.hpp"

namespace blockcraft
{

	void chunk::constructChunkData()
	{

		// TODO Turn this in to some proper rendering with noise maps.

				// Stone Layer
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

		// Stone Layer
		for (size_t x = 0; x < CHUNK_SIZE; x++)
		{
			for (size_t y = 0; y < CHUNK_HEIGHT - 1; y++)
			{
				for (size_t z = 0; z < CHUNK_SIZE; z++)
				{
					_blockData[x][y][z] = ID_BLOCK_STONE;
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

	void chunk::constructRendering()
	{
		_blockVertices.resize(0);
		_blockIndices.resize(0);

		uint32_t i = 0;
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

						// Skip rendering if it is not visible.
						if (!_visibleSides[z][y][x][s])
							continue;

						typedef blockSides sc;


						// Get required texture cords for current side.
						std::array<glm::vec2, 4> tex = _world->getBlockLibrary()->getTextureCoordSideFromId(_blockData[x][y][z], s);


						// Select which side to add based on index s.
						switch (s)
						{
						case 0: // Bottom
							_blockVertices.push_back(blockVertex({ sc::bottom[0].x + x, sc::bottom[0].y + y, sc::bottom[0].z + z }, tex[0]));
							_blockVertices.push_back(blockVertex({ sc::bottom[1].x + x, sc::bottom[1].y + y, sc::bottom[1].z + z }, tex[1]));
							_blockVertices.push_back(blockVertex({ sc::bottom[2].x + x, sc::bottom[2].y + y, sc::bottom[2].z + z }, tex[2]));
							_blockVertices.push_back(blockVertex({ sc::bottom[3].x + x, sc::bottom[3].y + y, sc::bottom[3].z + z }, tex[3]));
							break;

						case 1: // Top
							_blockVertices.push_back(blockVertex({ sc::top[0].x + x, sc::top[0].y + y, sc::top[0].z + z }, tex[0]));
							_blockVertices.push_back(blockVertex({ sc::top[1].x + x, sc::top[1].y + y, sc::top[1].z + z }, tex[1]));
							_blockVertices.push_back(blockVertex({ sc::top[2].x + x, sc::top[2].y + y, sc::top[2].z + z }, tex[2]));
							_blockVertices.push_back(blockVertex({ sc::top[3].x + x, sc::top[3].y + y, sc::top[3].z + z }, tex[3]));
							break;

						case 2: // Left
							_blockVertices.push_back(blockVertex({ sc::left[0].x + x, sc::left[0].y + y, sc::left[0].z + z }, tex[1]));
							_blockVertices.push_back(blockVertex({ sc::left[1].x + x, sc::left[1].y + y, sc::left[1].z + z }, tex[2]));
							_blockVertices.push_back(blockVertex({ sc::left[2].x + x, sc::left[2].y + y, sc::left[2].z + z }, tex[3]));
							_blockVertices.push_back(blockVertex({ sc::left[3].x + x, sc::left[3].y + y, sc::left[3].z + z }, tex[0]));
							break;

						case 3: // Right
							_blockVertices.push_back(blockVertex({ sc::right[0].x + x, sc::right[0].y + y, sc::right[0].z + z }, tex[1]));
							_blockVertices.push_back(blockVertex({ sc::right[1].x + x, sc::right[1].y + y, sc::right[1].z + z }, tex[2]));
							_blockVertices.push_back(blockVertex({ sc::right[2].x + x, sc::right[2].y + y, sc::right[2].z + z }, tex[3]));
							_blockVertices.push_back(blockVertex({ sc::right[3].x + x, sc::right[3].y + y, sc::right[3].z + z }, tex[0]));
							break;

						case 4: // Front
							_blockVertices.push_back(blockVertex({ sc::front[0].x + x, sc::front[0].y + y, sc::front[0].z + z }, tex[0]));
							_blockVertices.push_back(blockVertex({ sc::front[1].x + x, sc::front[1].y + y, sc::front[1].z + z }, tex[1]));
							_blockVertices.push_back(blockVertex({ sc::front[2].x + x, sc::front[2].y + y, sc::front[2].z + z }, tex[2]));
							_blockVertices.push_back(blockVertex({ sc::front[3].x + x, sc::front[3].y + y, sc::front[3].z + z }, tex[3]));
							break;

						case 5: // Back
							_blockVertices.push_back(blockVertex({ sc::back[0].x + x, sc::back[0].y + y, sc::back[0].z + z }, tex[2]));
							_blockVertices.push_back(blockVertex({ sc::back[1].x + x, sc::back[1].y + y, sc::back[1].z + z }, tex[3]));
							_blockVertices.push_back(blockVertex({ sc::back[2].x + x, sc::back[2].y + y, sc::back[2].z + z }, tex[0]));
							_blockVertices.push_back(blockVertex({ sc::back[3].x + x, sc::back[3].y + y, sc::back[3].z + z }, tex[1]));
							break;
						}

						// Update index buffer and add indices for 1 side.
						_blockIndices.push_back(i);
						_blockIndices.push_back(i + 1);
						_blockIndices.push_back(i + 2);
						_blockIndices.push_back(i + 2);
						_blockIndices.push_back(i + 3);
						_blockIndices.push_back(i);

						i += 4;
					}
				}
			}
		}
	}

	std::pair<float*, uint32_t> chunk::getChunkVertexData()
	{
		_hasNewVertexData = false;
		return { (float*)&_blockVertices[0], _blockVertices.size() * sizeof(blockVertex) / sizeof(float) };
	}

	std::pair<uint32_t*, uint32_t> chunk::getChunkIndexData()
	{
		_hasNewIndexData = false;
		return { (uint32_t*)&_blockIndices[0], _blockIndices.size() };
	}

	glm::mat4 chunk::getModelMatrix() const
	{
		// Create matrix with a translation to the right position.
		return glm::translate(glm::mat4(1.0f), glm::vec3(_chunkPosition.x * CHUNK_SIZE, 0, _chunkPosition.y * CHUNK_SIZE));
	}

	void chunk::setBlock(uint32_t x, uint32_t y, uint32_t z, uint32_t id)
	{
		// Enable new data.
		_hasNewVertexData = true;
		_hasNewIndexData = true;

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

		constructRendering();
	}

	inline void chunk::updateVisibilityDataForBlock(uint32_t x, uint32_t y, uint32_t z)
	{

		// Hide all sides if the block is air.
		if (_blockData[x][y][z] == ID_BLOCK_AIR)
		{
			return;
		}


		// Bottom
		if (y > 0)
			_visibleSides[z][y][x][0] = _blockData[x][y - 1][z] == ID_BLOCK_AIR;
		else
			_visibleSides[z][y][x][0] = true;

		// Top
		if (y < CHUNK_HEIGHT - 1)
			_visibleSides[z][y][x][1] = _blockData[x][y + 1][z] == ID_BLOCK_AIR;
		else
			_visibleSides[z][y][x][1] = true;

		// Left
		if (x > 0)
			_visibleSides[z][y][x][2] = _blockData[x - 1][y][z] == ID_BLOCK_AIR;
		else
			_visibleSides[z][y][x][2] = true;

		// Right
		if (x < CHUNK_SIZE - 1)
			_visibleSides[z][y][x][3] = _blockData[x + 1][y][z] == ID_BLOCK_AIR;
		else
			_visibleSides[z][y][x][3] = true;

		// Front
		if (z > 0)
			_visibleSides[z][y][x][4] = _blockData[x][y][z - 1] == ID_BLOCK_AIR;
		else
			_visibleSides[z][y][x][4] = true;

		// Back
		if (z < CHUNK_SIZE - 1)
			_visibleSides[z][y][x][5] = _blockData[x][y][z + 1] == ID_BLOCK_AIR;
		else
			_visibleSides[z][y][x][5] = true;

	}

}