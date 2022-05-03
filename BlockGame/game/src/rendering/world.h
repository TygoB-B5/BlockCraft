#pragma once

#include "engine.h"
#include "glm/glm.hpp"
#include <vector>
#include "block.h"
#include "chunk.h"


namespace blockcraft
{

	class world
	{
	public:

		world();

		~world();

		void init();

		void addChunk(const glm::vec2 cords);

		void draw(const spectatorCamera* camera, const glr::renderer* renderer);

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

		blockTextureLibrary _blockLibrary;
	};
}