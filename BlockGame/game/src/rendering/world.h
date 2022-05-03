#pragma once

#include "engine.h"
#include "chunk.h"
#include "glm/glm.hpp"
#include <vector>
#include "blocktexturelibrary.h"


namespace blockcraft
{
	class chunk;

	class world
	{
	public:

		world();

		~world();

		void init();

		chunk* addChunk(const glm::vec2 cords);

		void draw(const spectatorCamera* camera, const glr::renderer* renderer);

		const std::vector<chunk*>* getChunks() const
		{
			return &_chunks;
		}

		blockTextureLibrary* getBlockLibrary()
		{
			return &_blockLibrary;
		}


	private:

		std::vector<chunk*> _chunks;
		std::vector<glr::elementBuffer*> _chunkElementBuffers;
		std::vector<glr::vertexBuffer*> _chunkVertexBuffer;
		glr::vertexLayout<2> _vertexLayout;

		glr::shader _shader;
		static const char* _vertexShaderSrc;
		static const char* _fragmentShaderSrc;

		blockTextureLibrary _blockLibrary;
	};
}