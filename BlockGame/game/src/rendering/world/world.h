#pragma once


// NOTE:
// 
//		Do not change CHUNK_SIZE or CHUNK_HEIGHT higher than,
//		| UCHAR_MAX | 255 | 0xff |
//
//


#define CHUNK_HEIGHT 64
#define CHUNK_SIZE 16

#define MAX_WORLD_CHUNK_SIZE SHRT_MAX


#include "engine.h"
#include "block/blocktlibrary.h"
#include "chunk.h"
#include <map>


namespace blockcraft
{
	class chunk;

	class world
	{
	public:

		world(uint32_t seed);

		~world();

		// Initializes the world.
		void init();

		// Adds a new chunk to the world.
		chunk* addChunk(const glm::vec2& cords);

		// Removes a chunk from the world.
		void removeChunk(const glm::vec2& cords);

		// Renders the whole world.
		void draw(const glr::perspectiveCamera* camera, const glr::renderer* renderer);

		// Replaces a block on the given coordinate.
		void setBlock(int32_t x, uint8_t y, int32_t z, uint8_t id);

		// Returns the block of a given global coordinate.
		uint8_t getBlock(int32_t x, uint8_t y, int32_t z);

		// Returns the block of a given coordinate of a chunk in the local chunk coordinate system.
		uint8_t getBlockLocal(const glm::vec2& chunkPosition, int16_t x, uint8_t y, int16_t z);

		// Converts global coordinate to local chunk coordinate and gives a ptr to the chunk.
		void globalCoordToChunkCoord(chunk** chunkRef, int32_t* x, uint8_t* y, int32_t* z);

		// Returns all the chunks.
		const std::map<int, chunk*>* getChunks() const { return &_chunkMap; }

		// Returns the chunk from a given position.
		inline chunk* getChunkFromPosition(const glm::vec2& chunkPosition);

		// Returns the block library.
		blockTextureLibrary* getBlockLibrary()
		{
			return &_blockLibrary;
		}


		// Returns noise.
		glr::perlinnoise2d* getNoise()
		{
			return &_noise;
		}

		const glr::shader* getShader() const { return &_shader; }
		glr::vertexLayout<2>* getVertexLayout() { return &_vertexLayout; }
		const std::vector<glr::elementBuffer*> getElementBuffers() const;
		const std::vector<glr::vertexBuffer*> getVertexBuffers() const;


	private:
		// All the initialized chunks.
		std::map<int32_t, chunk*> _chunkMap;

		// Rendering data.
		std::map<chunk*, glr::elementBuffer*> _chunkElementBuffers;
		std::map<chunk*, glr::vertexBuffer*> _chunkVertexBuffer;

		glr::vertexLayout<2> _vertexLayout;
		glr::shader _shader;
		static const char* _vertexShaderSrc;
		static const char* _fragmentShaderSrc;


		// Texture library.
		blockTextureLibrary _blockLibrary;

		// World generation noise.
		glr::perlinnoise2d _noise;
	};
}