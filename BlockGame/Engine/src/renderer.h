#pragma once

#include "glad/glad.h"
#include <glfw/glfw3.h>
#include "stb_image.h"
#include <assert.h>
#include <array>
#include <vector>
#include "window.h"
#include "time.h"
#include "input.h"

namespace glr
{

	class shader
	{
	public:

		shader(const char* vertexSrc = nullptr, const char* fragmentSrc = nullptr);

		~shader();

		void compileShader(const char* vertexSrc, const char* fragmentSrc);

		void bind() const;

		void unbind();

		void setUniformMat4(const char* uniformName, const float* matrix);

	private:

		uint32_t _id;
	};


	class texture
	{
	public:

		texture(const char* filename);

		~texture();

		void setTexture(const char* filename);

		void bind(uint32_t slot) const;

		inline uint32_t getWidth() const { return _width; }

		inline uint32_t getHeight() const { return _height; }

	private:

		uint32_t _id;
		uint32_t _slot;

		uint32_t _width;
		uint32_t _height;
	};


	class vertexBuffer
	{
	public:

		vertexBuffer(const std::vector<float>& vertices);

		vertexBuffer(const float* vertices, uint32_t elements);

		~vertexBuffer();

		void setVertexData(const float* vertices, uint32_t elements);

		void bind() const;

		void unbind() const;

		uint32_t getVertexAmount() const { return _vertexAmount; }

	private:

		uint32_t _vboId;
		uint32_t _vaoId;

		uint32_t _vertexAmount;
	};


	template<int S>
	struct vertexLayout
	{
	public:

		void bind() const;

	public:

		std::array<uint32_t, S> Amount = { 0 };
		std::array<uint32_t, S> DataType = { 0 };

		uint32_t Stride = 0;
	};


	struct vertexLayoutDynamic
	{
	public:

		vertexLayoutDynamic(uint32_t initialSize = 0);

		void setSize(uint32_t size);

		void bind() const;

	public:

		std::vector<uint32_t> Amount = { 0 };
		std::vector<uint32_t> DataType = { 0 };

		uint32_t Stride = 0;
	};


	class elementBuffer
	{
	public:

		elementBuffer(const uint32_t* indices, uint32_t elements);

		elementBuffer(const std::vector<uint32_t>& indices);

		~elementBuffer();

		void setElementData(const uint32_t* indices, uint32_t elements);

		void bind() const;

		void unbind() const;

		uint32_t getElementAmount() const { return _elementAmount; }

	private:

		uint32_t _id;
		uint32_t _elementAmount;
	};


	class renderer
	{
	public:

		struct rendererSettings
		{
		public:

			enum class cullingMode
			{
				None = 0,
				Front = GL_FRONT,
				Back = GL_BACK,
			};

			enum class depthTestingMode
			{
				None = 0,
				Always = GL_ALWAYS,
				Never = GL_NEVER,
				Less = GL_LESS,
				Equal = GL_EQUAL,
				Greater = GL_GREATER
			};

		public:

			cullingMode CullingMode = cullingMode::Back;
			depthTestingMode DepthTestingMode = depthTestingMode::None;

			std::array<float, 4> ClearColor = { 0, 0, 0, 1 };
		};


		renderer(const window& window, const rendererSettings& settings);

		void init();

		void clear() const;

		void draw(uint32_t elements) const;

		void update();

		const window& getWindow() const { return _window; }

		const time& getTime() const { return _time; }

		const input& getInput() const { return _input; }
		input* getInput() { return &_input; }

	private:

		window _window;
		time _time;
		input _input;
		rendererSettings _renderSettings;
	};


	uint32_t glDatatypeSize(uint32_t type);

	// Cannot be included in the .cpp file, it causes a linking error.
	template<int S>
	inline void vertexLayout<S>::bind() const
	{

		uint32_t offset = 0;

		// Set stride and vertex layout.
		for (size_t i = 0; i < S; i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, Amount[i], DataType[i], GL_FALSE, Stride, (const void*)offset);

			offset += glDatatypeSize(DataType[i]) * Amount[i];
		}

	}


}