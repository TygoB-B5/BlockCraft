#pragma once

#include "engine.h"
#include "glm/glm.hpp"
#include "camera/spectatorcamera.h"
#include <vector>
#include <utility>
#include <array>

#define BLOCK_AIR 0
#define BLOCK_DIRT 1

#define CHUNK_HEIGHT 32
#define CHUNK_SIZE 32


#define BLOCK_TEXTURE_FILE_NAME "minecraft.jpg"
#define BLOCK_TEXTURE_PIXEL_SIZE 16


namespace blockgame
{

	struct blockVertex
	{

		blockVertex(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec2 texCoord = { 0.0f, 0.0f })
			: Position(position), TexCoord(texCoord)
		{}


		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	struct sideConstants
	{
		static blockVertex bottom[4];
		static blockVertex top[4];
		static blockVertex left[4];
		static blockVertex right[4];
		static blockVertex front[4];
		static blockVertex back[4];
	};

	class chunk
	{
	public:

		chunk(const glm::vec2& cord)
			: _chunkPosition(cord)
		{
			constructChunkData();
			calculateAllBlockVisibility();
			constructRendering();
		}

		void constructChunkData()
		{
			// dirt layer
			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				for (size_t y = 0; y < CHUNK_HEIGHT / 2; y++)
				{
					for (size_t z = 0; z < CHUNK_SIZE; z++)
					{
						_blockData[x][y][z] = BLOCK_DIRT;
					}
				}
			}

			// air layer
			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				for (size_t y = CHUNK_HEIGHT / 2; y < CHUNK_HEIGHT; y++)
				{
					for (size_t z = 0; z < CHUNK_SIZE; z++)
					{
						_blockData[x][y][z] = BLOCK_AIR;
					}
				}
			}

			// rand layer
			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				for (size_t y = CHUNK_HEIGHT / 2; y < CHUNK_HEIGHT / 2 + 1; y++)
				{
					for (size_t z = 0; z < CHUNK_SIZE; z++)
					{
						_blockData[x][y][z] = rand() % 3 == 2 ? BLOCK_AIR : BLOCK_DIRT;
					}
				}
			}
		}


		void calculateAllBlockVisibility()
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


		void constructRendering()
		{
			_blockVertices.clear();
			_blockIndices.clear();

			uint32_t i = 0;
			for (size_t x = 0; x < CHUNK_SIZE; x++)
			{
				for (size_t y = 0; y < CHUNK_HEIGHT; y++)
				{
					for (size_t z = 0; z < CHUNK_SIZE; z++)
					{

						// Skip loop if block is air.
						if (_blockData[x][y][z] == BLOCK_AIR)
							continue;


						// Add sides that need to be rendered.
						for (size_t s = 0; s < 6; s++)
						{

							// Skip rendering if it is not visable anyways.
							 if (!_visibleSides[z][y][x][s])
								continue;

							typedef sideConstants sc;

							// Select which side to add based on index s.
							switch (s)
							{
							case 0:
								_blockVertices.push_back(blockVertex({ sc::bottom[0].Position.x + x, sc::bottom[0].Position.y + y, sc::bottom[0].Position.z + z }, { 0.0f, 0.0f }));
								_blockVertices.push_back(blockVertex({ sc::bottom[1].Position.x + x, sc::bottom[1].Position.y + y, sc::bottom[1].Position.z + z }, { 0.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::bottom[2].Position.x + x, sc::bottom[2].Position.y + y, sc::bottom[2].Position.z + z }, { 1.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::bottom[3].Position.x + x, sc::bottom[3].Position.y + y, sc::bottom[3].Position.z + z }, { 1.0f, 0.0f }));
								break;

							case 1:
								_blockVertices.push_back(blockVertex({ sc::top[0].Position.x + x, sc::top[0].Position.y + y, sc::top[0].Position.z + z }, { 0.0f, 0.0f }));
								_blockVertices.push_back(blockVertex({ sc::top[1].Position.x + x, sc::top[1].Position.y + y, sc::top[1].Position.z + z }, { 0.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::top[2].Position.x + x, sc::top[2].Position.y + y, sc::top[2].Position.z + z }, { 1.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::top[3].Position.x + x, sc::top[3].Position.y + y, sc::top[3].Position.z + z }, { 1.0f, 0.0f }));
								break;

							case 2:
								_blockVertices.push_back(blockVertex({ sc::left[0].Position.x + x, sc::left[0].Position.y + y, sc::left[0].Position.z + z }, { 0.0f, 0.0f }));
								_blockVertices.push_back(blockVertex({ sc::left[1].Position.x + x, sc::left[1].Position.y + y, sc::left[1].Position.z + z }, { 0.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::left[2].Position.x + x, sc::left[2].Position.y + y, sc::left[2].Position.z + z }, { 1.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::left[3].Position.x + x, sc::left[3].Position.y + y, sc::left[3].Position.z + z }, { 1.0f, 0.0f }));
								break;

							case 3:
								_blockVertices.push_back(blockVertex({ sc::right[0].Position.x + x, sc::right[0].Position.y + y, sc::right[0].Position.z + z }, { 0.0f, 0.0f }));
								_blockVertices.push_back(blockVertex({ sc::right[1].Position.x + x, sc::right[1].Position.y + y, sc::right[1].Position.z + z }, { 0.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::right[2].Position.x + x, sc::right[2].Position.y + y, sc::right[2].Position.z + z }, { 1.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::right[3].Position.x + x, sc::right[3].Position.y + y, sc::right[3].Position.z + z }, { 1.0f, 0.0f }));
								break;

							case 4:
								_blockVertices.push_back(blockVertex({ sc::front[0].Position.x + x, sc::front[0].Position.y + y, sc::front[0].Position.z + z }, { 0.0f, 0.0f }));
								_blockVertices.push_back(blockVertex({ sc::front[1].Position.x + x, sc::front[1].Position.y + y, sc::front[1].Position.z + z }, { 0.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::front[2].Position.x + x, sc::front[2].Position.y + y, sc::front[2].Position.z + z }, { 1.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::front[3].Position.x + x, sc::front[3].Position.y + y, sc::front[3].Position.z + z }, { 1.0f, 0.0f }));
								break;

							case 5:
								_blockVertices.push_back(blockVertex({ sc::back[0].Position.x + x, sc::back[0].Position.y + y, sc::back[0].Position.z + z }, { 0.0f, 0.0f }));
								_blockVertices.push_back(blockVertex({ sc::back[1].Position.x + x, sc::back[1].Position.y + y, sc::back[1].Position.z + z }, { 0.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::back[2].Position.x + x, sc::back[2].Position.y + y, sc::back[2].Position.z + z }, { 1.0f, 1.0f }));
								_blockVertices.push_back(blockVertex({ sc::back[3].Position.x + x, sc::back[3].Position.y + y, sc::back[3].Position.z + z }, { 1.0f, 0.0f }));
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


		std::pair<float*, uint32_t> getChunkvertexData()
		{
			return { (float*)&_blockVertices[0], _blockVertices.size() * sizeof(blockVertex) / sizeof(float)};
		}

		std::pair<uint32_t*, uint32_t> getChunkIndexData()
		{
			return { (uint32_t*)&_blockIndices[0], _blockIndices.size()};
		}

		glm::mat4 getModelMatrix() const
		{
			return glm::translate(glm::mat4(1.0f), glm::vec3(_chunkPosition.x * CHUNK_SIZE, 0, _chunkPosition.y * CHUNK_SIZE));
		}

		inline void setBlockData(uint32_t x, uint32_t y, uint32_t z, uint32_t value)
		{
			_blockData[x][y][z] = value;

			updateVisibilityDataForBlock(x, y, z);


			//TODO Make this more efficient.
			//
			// Now it recalculates every vertex every time you place a block, this is innefficient.
			// Make it so it only updates the vertices around it.
			//

			constructRendering();
		}

		inline void updateVisibilityDataForBlock(uint32_t x, uint32_t y, uint32_t z)
		{
			// Skip if the block is air.
			if (_blockData[x][y][z] == BLOCK_AIR)
				return;

			// Bottom
			if (y >= 1)
				_visibleSides[z][y][x][0] = _blockData[x][y - 1][z] == BLOCK_AIR;
			else
				_visibleSides[z][y][x][0] = true;

			// Top
			if (y < CHUNK_HEIGHT - 1)
				_visibleSides[z][y][x][1] = _blockData[x][y + 1][z] == BLOCK_AIR;
			else
				_visibleSides[z][y][x][1] = true;

			// Left
			if (x >= 1)
				_visibleSides[z][y][x][2] = _blockData[x - 1][y][z] == BLOCK_AIR;
			else
				_visibleSides[z][y][x][2] = true;

			// Right
			if (x < CHUNK_SIZE - 1)
				_visibleSides[z][y][x][3] = _blockData[x + 1][y][z] == BLOCK_AIR;
			else
				_visibleSides[z][y][x][3] = true;

			// Front
			if (z > 0)
				_visibleSides[z][y][x][4] = _blockData[x][y][z - 1] == BLOCK_AIR;
			else
				_visibleSides[z][y][x][4] = true;

			// Back
			if (z < CHUNK_SIZE - 1)
				_visibleSides[z][y][x][5] = _blockData[x][y][z + 1] == BLOCK_AIR;
			else
				_visibleSides[z][y][x][5] = true;

		}


	private:

		glm::vec2 _chunkPosition;

		std::vector<blockVertex> _blockVertices;
		std::vector<uint32_t> _blockIndices;

		bool _visibleSides[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE][6] = { false };
		uint32_t _blockData[CHUNK_SIZE][CHUNK_HEIGHT][CHUNK_SIZE];
	};


	class world
	{
	public:

		world()
			: _texture(BLOCK_TEXTURE_FILE_NAME), _atlas(&_texture, BLOCK_TEXTURE_PIXEL_SIZE)
		{
			init();
		}

		~world()
		{

			// Delete all heap objects.
			for (auto& chunk : _chunks)
				delete(chunk);

			for (auto& elemBuffer : _chunkElementBuffers)
				delete(elemBuffer);

			for (auto& vertBuffer : _chunkVertexBuffer)
				delete(vertBuffer);

			// Clear vector elements.
			_chunkElementBuffers.clear();
			_chunkVertexBuffer.clear();
			_chunks.clear();

		}

		void init()
		{

			// Setup vertex Layout
			_vertexLayout.Amount[0] = 3;
			_vertexLayout.Amount[1] = 2;
			_vertexLayout.DataType[0] = GL_FLOAT;
			_vertexLayout.DataType[1] = GL_FLOAT;
			_vertexLayout.Stride = 5 * sizeof(float);


			// Setup shader.
			_shader.compileShader(_vertexShaderSrc, _fragmentShaderSrc);


			// Setup texture.
			_texture.setTexture("test2.jpg");


			// Setup textureAtlas.
			_atlas.set(&_texture, 16);

		}

		void addChunk(const glm::vec2 cords)
		{
			chunk* c = new chunk(cords);
			_chunks.push_back(c);
			_chunkElementBuffers.push_back(new glr::elementBuffer(c->getChunkIndexData().first, c->getChunkIndexData().second));
			_chunkVertexBuffer.push_back(new glr::vertexBuffer(c->getChunkvertexData().first, c->getChunkvertexData().second));
		}

		void draw(const spectatorCamera* camera, const glr::renderer* renderer)
		{

			// Bind vertex layout.
			_vertexLayout.bind();


			// Bind texture.
			_texture.bind(0);


			// Get Viewprojection matrix.
			glm::mat4 viewProjection = camera->getViewProjectionMatrix();

			// Draw chunks.
			for (size_t i = 0; i < _chunks.size(); i++)
			{

				// Bind element and vertex buffer
				_shader.bind();
				_chunkVertexBuffer[i]->bind();
				_chunkElementBuffers[i]->bind();

				_vertexLayout.bind();

				// Set ModelMatrix and ViewMatrix.
				glm::mat4 model = _chunks[i]->getModelMatrix();
				_shader.setUniformMat4("uViewMatrix", &viewProjection[0][0]);
				_shader.setUniformMat4("uModelMatrix", &model[0][0]);


				// Draw elements
				renderer->draw(_chunkElementBuffers[i]->getElementAmount());


				// Unbind.
				_shader.unbind();
				_chunkVertexBuffer[i]->unbind();
				_chunkElementBuffers[i]->unbind();

			}
		}

		chunk* getChunk() const
		{
			return _chunks[0];
		}


	private:

		std::vector<chunk*> _chunks;
		std::vector<glr::elementBuffer*> _chunkElementBuffers;
		std::vector<glr::vertexBuffer*> _chunkVertexBuffer;
		glr::vertexLayout<2> _vertexLayout;

		glr::shader _shader;
		static const char* _vertexShaderSrc;
		static const char* _fragmentShaderSrc;

		glr::texture _texture;
		glr::textureAtlas _atlas;
	};
}