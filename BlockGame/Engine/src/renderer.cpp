#include "renderer.h"
#include <iostream>

namespace glr
{

	/// Renderer

	renderer::renderer(const window& window, const rendererSettings& settings)
		: _window(window), _renderSettings(settings), _time(time()), _input(glr::input(&_window))
	{
		init();
	}

	// Initialize the renderer
	void renderer::init()
	{

		// Initialize GLAD.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			assert(false, "Failed to initialize GLAD");
		}


		// Get window width and height.
		int width = 0;
		int height = 0;
		glfwGetWindowSize(_window.getGlfwWindow(), &width, &height);

		assert(!(width <= 0 || height <= 0), "Window size is 0.");


		// Set viewport to window size.
		glViewport(0, 0, width, height);


		// Set Culling mode
		if (_renderSettings.CullingMode != rendererSettings::cullingMode::None)
		{
			glEnable(GL_CULL_FACE);
			glCullFace((GLenum)_renderSettings.CullingMode);
		}


		// Set depth testing mode.
		if (_renderSettings.DepthTestingMode != rendererSettings::depthTestingMode::None)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc((GLenum)_renderSettings.DepthTestingMode);
		}

	}

	void renderer::clear() const
	{

		// Clear buffer data.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set background color
		glClearColor(_renderSettings.ClearColor[0],
			_renderSettings.ClearColor[1],
			_renderSettings.ClearColor[2],
			_renderSettings.ClearColor[3]);

	}

	void renderer::draw(uint32_t elements) const
	{
		// Draw Triangles
		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
	}

	void renderer::update()
	{
		_time.update();
		_input.clear();
		_window.update();
	}


	/// Shader


	shader::shader(const char* vertexSrc, const char* fragmentSrc)
		: _id(0)
	{

		// Compile shader if src is given.
		if (!(vertexSrc == nullptr && fragmentSrc == nullptr))
		{
			compileShader(vertexSrc, fragmentSrc);
		}

	}

	shader::~shader()
	{
		// Delete shader.
		glDeleteProgram(_id);
	}

	void shader::compileShader(const char* vertexSrc, const char* fragmentSrc)
	{

		// Create a vertex shader object
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);


		// Attach the shader source code to the shader object.
		glShaderSource(vertexShader, 1, &vertexSrc, NULL);


		// Compile the vertex shader dynamically.
		glCompileShader(vertexShader);



		// Check if compilation was successful.
		GLint success = false;
		char infoLog[512];

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
			std::cout << infoLog << std::endl;
		}


		// Create a fragment shader object.
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


		// Attach the shader source code to the shader object.
		glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);


		// Compile the fragment shader dynamically.
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
			std::cout << infoLog << std::endl;
		}


		// Create a shader program.
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();


		// Attach the compiled shaders to the shader program.
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Check if linking was successful.
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINKING_FAILED\n";
			std::cout << infoLog << std::endl;
		}


		// Assign shaderProgram to renderer.
		_id = shaderProgram;


		// Delete shader objects if we no longer need them anymore.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

	}

	void shader::bind() const
	{
		assert(!(_id == 0), "Shader is not compiled.");

		// Bind
		glUseProgram(_id);
	}

	void shader::unbind()
	{
		// Unbind shader.
		glUseProgram(0);
	}


	void shader::setUniformMat4(const char* uniformName, const float* matrix)
	{
		// Get uniform id and upload uniform.
		GLuint uniformId = glGetUniformLocation(_id, uniformName);
		glUniformMatrix4fv(uniformId, 1, GL_FALSE, matrix);
	}


	/// Texture


	texture::texture(const char* filename)
		: _width(0), _height(0), _slot(0)
	{

		// Create texture.
		glCreateTextures(GL_TEXTURE_2D, 1, &_id);


		// Set texture if filename exists
		if (filename)
		{
			setTexture(filename);
		}
	}

	texture::~texture()
	{
		// Delete texture.
		glDeleteTextures(1, &_id);
	}

	void texture::setTexture(const char* filename)
	{

		// Load image.
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

		assert(!(data == NULL), "Texture file not found!");

		// Initialize and bind texture to textureslot 0
		glBindTexture(GL_TEXTURE_2D, _id);


		// Set texture filter settings.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		// Save general info from the texture.
		_width = width;
		_height = height;


		// Convert channel count to Gl Texture channel.
		GLenum internalFormat = 0;
		GLenum dataFormat = 0;

		if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}


		// Set texture data.
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		// Free image data.
		stbi_image_free(data);
	}

	void texture::bind(uint32_t slot) const
	{
		// Bind texture.
		glBindTextureUnit(slot, _id);
	}


	/// Vertexbuffer


	vertexBuffer::vertexBuffer(const std::vector<float>& vertices)
		: _vboId(0), _vaoId(0), _vertexAmount(0)
	{

		// Generate buffer and array.
		glGenVertexArrays(1, &_vaoId);
		glGenBuffers(1, &_vboId);

		// Set vertex data if used as a parameter.
		if (!vertices.empty())
		{
			setVertexData(&vertices[0], (uint32_t)vertices.size());
		}

	}

	vertexBuffer::vertexBuffer(const float* vertices, uint32_t elements)
		: _vboId(0), _vaoId(0), _vertexAmount(0)
	{

		// Generate buffer and array.
		glGenVertexArrays(1, &_vaoId);
		glGenBuffers(1, &_vboId);

		// Set vertex data if used as a parameter.
		if (vertices)
		{
			setVertexData(vertices, elements);
		}

	}

	vertexBuffer::~vertexBuffer()
	{
		// Delete buffer and vertex array
		glDeleteBuffers(1, &_vboId);
		glDeleteVertexArrays(1, &_vaoId);
	}

	void vertexBuffer::setVertexData(const float* vertices, uint32_t elements)
	{

		// Bind VAO, then bind VBO.
		glBindBuffer(GL_ARRAY_BUFFER, _vboId);
		bind();


		// Copy the vertex data into the buffer's memory
		glBufferData(GL_ARRAY_BUFFER, elements * sizeof(float), vertices, GL_DYNAMIC_DRAW);

		_vertexAmount = elements;
	}

	// Binds the vertex array.
	void vertexBuffer::bind() const
	{
		// Bind vao
		glBindBuffer(GL_ARRAY_BUFFER, _vboId);
		glBindVertexArray(_vaoId);
	}

	void vertexBuffer::unbind() const
	{
		// Unbind buffer.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	/// Vertexlayout(dynamic)


	vertexLayoutDynamic::vertexLayoutDynamic(uint32_t initialSize)
	{
		setSize(initialSize);
	}

	void vertexLayoutDynamic::setSize(uint32_t size)
	{
		// Resize vectors to specified size.
		Amount.resize(size);
		DataType.resize(size);
	}

	void vertexLayoutDynamic::bind() const
	{

		uint32_t offset = 0;

		// Set stride and vertex layout.
		for (uint32_t i = 0; i < Amount.size(); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, Amount[i], DataType[i], GL_FALSE, Stride, (const void*)offset);

			offset += glDatatypeSize(DataType[i]) * Amount[i];
		}

	}



	/// Elementbuffer


	elementBuffer::elementBuffer(const uint32_t* indices, uint32_t elements)
		: _id(0), _elementAmount(0)
	{

		// Generate buffer.
		glGenBuffers(1, &_id);

		// Set vertex data if used as a parameter.
		if (indices)
		{
			setElementData(indices, elements);
		}

	}

	elementBuffer::elementBuffer(const std::vector<uint32_t>& indices)
		: _id(0), _elementAmount(0)
	{

		// Generate buffer.
		glGenBuffers(1, &_id);

		// Set vertex data if used as a parameter.
		if (!indices.empty())
		{
			setElementData(&indices[0], (uint32_t)indices.size());
		}

	}

	elementBuffer::~elementBuffer()
	{
		// Delete buffer.
		glDeleteBuffers(1, &_id);
	}

	void elementBuffer::setElementData(const uint32_t* indices, uint32_t elements)
	{

		// Bind EBO
		bind();

		// Copy the index data into the buffer's memory
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);

		_elementAmount = elements;
	}

	// Binds the element array.
	void elementBuffer::bind() const
	{
		// Bind vao
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void elementBuffer::unbind() const
	{
		// Bind vao
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t glDatatypeSize(uint32_t type)
	{

		// Return size in bytes from Opengl datatype.
		switch (type)
		{
		case GL_FLOAT:        return 4;
		case GL_INT:          return 4;
		case GL_DOUBLE:       return 8;
		case GL_UNSIGNED_INT: return 4;

		default: assert(false, "Datatype not implemented."); return 0;
		}

	}

}