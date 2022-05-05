#pragma once

#include "engine.h"
#include "chunk.h"
#include "glm/glm.hpp"
#include <vector>
#include "block/blocktexturelibrary.h"


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

		void setBlock(uint32_t x, uint32_t y, uint32_t z, uint32_t id);

		const std::vector<chunk*>* getChunks() const { return &_chunks; }

		chunk* getChunkFromPosition(const glm::vec2& chunkPosition);

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
		std::vector<chunk*> _chunks;

		// Rendering data.
		std::vector<glr::elementBuffer*> _chunkElementBuffers;
		std::vector<glr::vertexBuffer*> _chunkVertexBuffer;
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