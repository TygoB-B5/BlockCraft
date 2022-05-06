#include "world.h"

namespace blockcraft
{	

	const char* world::_vertexShaderSrc = R"(

		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoord;

		out vec2 oTexCoord;
		out float oDepth;
		uniform mat4 uViewMatrix;
		uniform mat4 uModelMatrix;

		void main()
		{
			vec4 pos = uViewMatrix * uModelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
			gl_Position = pos;
			oTexCoord = aTexCoord;
			oDepth = pos.z;
		}

		)";



	const char* world::_fragmentShaderSrc = R"(

		#version 330 core
		out vec4 FragColor;

		in vec2 oTexCoord;
		in float oDepth;
		uniform sampler2D uTexture;
		

		void main()
		{
			float FOG_DISTANCE = 3024;

			float intensity = clamp(oDepth / FOG_DISTANCE, 0, 1); 
			vec4 col = mix( texture(uTexture, oTexCoord), vec4(0.65f, 0.75f, 1.0f, 1), intensity );
			
			FragColor = col;
		}

)";


	// World


	world::world(uint32_t seed)
		: _blockLibrary(blockTextureLibrary()),
		_noise(seed, 512, 5, 0.5)
	{
		GLR_ASSERT(seed > 2, "Seed must be higher than 4");
		init();
	}

	world::~world()
	{

		// Delete all heap objects.
		for (auto& chunk : _chunkMap)
			delete(chunk.second);

		for (auto& elemBuffer : _chunkElementBuffers)
			delete(elemBuffer.second);

		for (auto& vertBuffer : _chunkVertexBuffer)
			delete(vertBuffer.second);

		// Clear vector elements.
		_chunkElementBuffers.clear();
		_chunkVertexBuffer.clear();
		_chunkMap.clear();

	}

	void world::init()
	{

		// Setup vertex Layout
		_vertexLayout.Amount[0]   = 3;
		_vertexLayout.Amount[1]   = 2;
		_vertexLayout.DataType[0] = GL_FLOAT;
		_vertexLayout.DataType[1] = GL_FLOAT;
		_vertexLayout.Stride      = 5 * sizeof(float);


		// Setup shader.
		_shader.compileShader(_vertexShaderSrc, _fragmentShaderSrc);

	}

	chunk* world::addChunk(const glm::vec2& cords)
	{

#ifdef GLR_DEBUG

		// Check if chunk does not already exist on position
		if(_chunkMap.find(cords.x * MAX_WORLD_CHUNK_SIZE + cords.y) != _chunkMap.end())
			GLR_ASSERT(false, "Chunk already exists on position.");

#endif

		// Create chunk and add all the buffers and chunk.
		chunk* c = new chunk(cords, this);
		_chunkMap.insert(std::pair<int, chunk*>(cords.x * MAX_WORLD_CHUNK_SIZE + cords.y, c));

		
		// Initialize the chunk after the object has been added to the chunk array.
		c->init();


		// Create element and vertex buffer with the chunk data.
		_chunkElementBuffers.insert(std::pair<chunk*, glr::elementBuffer*>(c, new glr::elementBuffer(c->getChunkIndexData().first, c->getChunkIndexData().second)));
		_chunkVertexBuffer.insert(std::pair<chunk*, glr::vertexBuffer*>(c, new glr::vertexBuffer(c->getChunkVertexData().first, c->getChunkVertexData().second)));

		return c;
	}

	void world::removeChunk(const glm::vec2& cords)
	{

		// Get chunk.
		chunk* chunk = getChunkFromPosition(cords);


		// If chunk does not exist at cords return;
		if (!chunk)
		{
			return;
		}


		// Free chunk elembuffer and vertbuffer memory.
		delete(chunk);
		delete(_chunkElementBuffers[chunk]);
		delete(_chunkVertexBuffer[chunk]);

		// Clear maps.
		_chunkMap.erase(cords.x * MAX_WORLD_CHUNK_SIZE + cords.y);
		_chunkElementBuffers.erase(chunk);
		_chunkVertexBuffer.erase(chunk);

		return;
	}

	void world::draw(const glr::perspectiveCamera* camera, const glr::renderer* renderer)
	{

		// Bind vertex layout.
		_vertexLayout.bind();


		// Bind texture.
		_blockLibrary.getTexture().bind(0);


		// Get Viewprojection matrix.
		glm::mat4 viewProjection = camera->getViewProjectionMatrix();


		// Draw chunks.
		for (auto& chunkMap : _chunkMap)
		{
			chunk* c = chunkMap.second;

			// Bind element and vertex buffer
			_shader.bind();

			// Get new vertex and index data if there is any.
			if(c->getHasNewVertexData())
				_chunkVertexBuffer[c]->setVertexData(c->getChunkVertexData().first, c->getChunkVertexData().second);

			if (c->getHasNewIndexData())
				_chunkElementBuffers[c]->setElementData(c->getChunkIndexData().first, c->getChunkIndexData().second);

			_chunkVertexBuffer[c]->bind();
			_chunkElementBuffers[c]->bind();

			_vertexLayout.bind();


			// Set ModelMatrix and ViewMatrix.
			glm::mat4 model = c->getModelMatrix();
			_shader.setUniformMat4("uViewMatrix", &viewProjection[0][0]);
			_shader.setUniformMat4("uModelMatrix", &model[0][0]);


			// Draw elements
			renderer->draw(_chunkElementBuffers[c]->getElementAmount());

		}
	}

	void world::setBlock(int32_t x, uint8_t  y, int32_t z, uint8_t id)
	{
		chunk* c = nullptr;
		coordToChunkCoord(&c, &x, &y, &z);

		if (c)
		{
			c->setBlock(x, y, z, id);
		}

	}

	uint8_t world::getBlock(int32_t x, uint8_t y, int32_t z)
	{
		chunk* c = nullptr;
		coordToChunkCoord(&c, &x, &y, &z);

		if (c)
		{
			return c->getBlock(x, y, z);
		}

	}

	// Chunk ptr validates if position is valid
	void world::coordToChunkCoord(chunk** chunkRef, int32_t* x, uint8_t* y, int32_t* z)
	{

#ifdef GLR_DEBUG

		if (abs(*x) > MAX_WORLD_CHUNK_SIZE * CHUNK_SIZE ||
			abs(*z) > MAX_WORLD_CHUNK_SIZE * CHUNK_SIZE)
			GLR_ASSERT(false, "Block position outside of the world.")

#endif

			// Ignore if block is placed above the maximum chunk height.
			if (*y >= CHUNK_HEIGHT)
				return;


		// Get positive chunk position data.
		int16_t chunkX = abs(*x / CHUNK_SIZE);
		int16_t chunkZ = abs(*z / CHUNK_SIZE);


		// If position is negative reverse the chunk position. (Done this way because negative division rounding is wacky)
		chunkX = *x < 0 ? -chunkX - 1 : chunkX;
		chunkZ = *z < 0 ? -chunkZ - 1 : chunkZ;

		// Set block on right chunk based on data.
		chunk* chunk = getChunkFromPosition({ chunkX, chunkZ });

		GLR_CORE_ASSERT(chunk, "Block position outside of loaded chunk range");

		if (chunk)
		{

			// Set chunkRef to chunk.
			*chunkRef = chunk;

			// Create chunk positions and mod by the chunk size.
			int8_t xChunkPos = *x % CHUNK_SIZE;
			int8_t zChunkPos = *z % CHUNK_SIZE;


			// Convert to the right chunk coordinates if the value is negative.
			*x = xChunkPos < 0 ? CHUNK_SIZE - (-xChunkPos) : xChunkPos;
			*z = zChunkPos < 0 ? CHUNK_SIZE - (-zChunkPos) : zChunkPos;
		}

	}

	chunk* world::getChunkFromPosition(const glm::vec2& chunkPosition)
	{

		// Check if chunk exists at position.
		if (_chunkMap.find(chunkPosition.x * MAX_WORLD_CHUNK_SIZE + chunkPosition.y) == _chunkMap.end())
			return nullptr;


		// Return chunk from position.
		return _chunkMap[chunkPosition.x * MAX_WORLD_CHUNK_SIZE + chunkPosition.y];
	}
}