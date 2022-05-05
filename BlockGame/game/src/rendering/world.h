#pragma once

#include "engine.h"
#include "glm/glm.hpp"
#include "block/blocktexturelibrary.h"
#include "chunk.h"
#include <map>

#define MAX_WORLD_CHUNK_SIZE SHRT_MAX

namespace blockcraft
{
	class chunk;

	class world
	{
	public:

		world(uint32_t seed);

		~world();

		void init();

		chunk* addChunk(const glm::vec2& cords);
		void removeChunk(const glm::vec2& cords);

		void draw(const spectatorCamera* camera, const glr::renderer* renderer);

		void setBlock(int32_t x, uint8_t y, int32_t z, uint8_t id);

		const std::map<int, chunk*>* getChunks() const { return &_chunkMap; }

		inline chunk* getChunkFromPosition(const glm::vec2& chunkPosition);

		blockTextureLibrary* getBlockLibrary()
		{
			return &_blockLibrary;
		}

		glr::perlinnoise2d* getNoise()
		{
			return &_noise;
		}


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