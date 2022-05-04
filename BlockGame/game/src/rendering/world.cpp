#include "world.h"

namespace blockcraft
{

	const char* world::_vertexShaderSrc = R"(

		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 oTexCoord;
		uniform mat4 uViewMatrix;
		uniform mat4 uModelMatrix;

		void main()
		{
			vec4 pos = uViewMatrix * uModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			gl_Position = pos;
			oTexCoord = aTexCoord;
		}

		)";



	const char* world::_fragmentShaderSrc = R"(

		#version 330 core
		out vec4 FragColor;

		in vec2 oTexCoord;
		uniform sampler2D uTexture;

		void main()
		{
			FragColor = texture(uTexture, oTexCoord);
		}

)";


	// World


	world::world()
		: _blockLibrary(blockTextureLibrary())
	{
		init();
	}

	world::~world()
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

	void world::init()
	{

		// Setup vertex Layout
		_vertexLayout.Amount[0] = 3;
		_vertexLayout.Amount[1] = 2;
		_vertexLayout.DataType[0] = GL_FLOAT;
		_vertexLayout.DataType[1] = GL_FLOAT;
		_vertexLayout.Stride = 5 * sizeof(float);


		// Setup shader.
		_shader.compileShader(_vertexShaderSrc, _fragmentShaderSrc);

	}

	chunk* world::addChunk(const glm::vec2 cords)
	{

		// Create chunk and add all the buffers and chunk.
		chunk* c = new chunk(cords, this);
		_chunks.push_back(c);

		
		// Initialize the chunk after the object has been added to the chunk array.
		c->init();


		// Create element and vertex buffer with the chunk data.
		_chunkElementBuffers.push_back(new glr::elementBuffer(c->getChunkIndexData().first, c->getChunkIndexData().second));
		_chunkVertexBuffer.push_back(new glr::vertexBuffer(c->getChunkVertexData().first, c->getChunkVertexData().second));


		return c;
	}

	void world::draw(const spectatorCamera* camera, const glr::renderer* renderer)
	{

		// Bind vertex layout.
		_vertexLayout.bind();


		// Bind texture.
		_blockLibrary.getTexture().bind(0);


		// Get Viewprojection matrix.
		glm::mat4 viewProjection = camera->getViewProjectionMatrix();


		// Draw chunks.
		for (size_t i = 0; i < _chunks.size(); i++)
		{

			// Bind element and vertex buffer
			_shader.bind();

			// Get new vertex and index data if there is any.
			if(_chunks[i]->getHasNewVertexData())
				_chunkVertexBuffer[i]->setVertexData(_chunks[i]->getChunkVertexData().first, _chunks[i]->getChunkVertexData().second);

			if (_chunks[i]->getHasNewIndexData())
				_chunkElementBuffers[i]->setElementData(_chunks[i]->getChunkIndexData().first, _chunks[i]->getChunkIndexData().second);

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

	void world::setBlock(uint32_t x, uint32_t y, uint32_t z, uint32_t id)
	{

		// Get chunk position data.
		uint32_t chunkX = x / CHUNK_SIZE;
		uint32_t chunkZ = z / CHUNK_SIZE;

		// Set block on right chunk based on data.
		for (chunk*& chunk : _chunks)
		{
			
			// if chunkposition matches
			if (chunk->getChunkPosition().x == chunkX
				&& chunk->getChunkPosition().y == chunkZ)
			{
				chunk->setBlock(x % CHUNK_SIZE, y, z % CHUNK_SIZE, id);

				return;
			}

		}

	}

	chunk* world::getChunkFromPosition(const glm::vec2& chunkPosition)
	{

		// Get chunk if available at position
		for (chunk*& chunk : _chunks)
		{
			if (chunkPosition == chunk->getChunkPosition())
				return chunk;
		}

		return nullptr;
	}
}