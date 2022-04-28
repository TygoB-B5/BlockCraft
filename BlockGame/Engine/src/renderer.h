#pragma once


/*
	Things I might wanna change:

	Not sure if using a renderer object is neccesary it might as well all be static or Singleton.
	Change to a more OOP approach.


-----------------------------------------------------------------------
	I have no idea how I want to do to abstract this renderer lmao.

	Some ideas:

	The draw process:
	1. Set the vertex layout.
	2. Gather all the required vertex data. (e.g. the sides of blocks with texcoords and albedo. Maybe also normals but I dont know what they would be useful for doee.)
	3. Bind the neccesary shader.
	4. Bind the neccesary textures. (using texture slots might be handy but I dont think it is neccesary if you use a texture atlas.)
	5. Draw all elements

	Need to deal with the Input bs.

*/


#include "glad/glad.h"
#include <glfw/glfw3.h>
#include "glm/glm.hpp"
#include <iostream>
#include <assert.h>
#include <array>
#include <string>
#include "stb_image.h"
#include <filesystem>




#ifdef _DEBUG
	#define DEBUG_ONLY(expression) expression
#else
	#define DEBUG_ONLY(expression)
#endif








namespace engine
{
	class window
	{
	public:

		window(uint32_t width, uint32_t height, const char* name)
		{
			// Initialize GLFW.
			glfwInit();


			// Create window.
			_window = glfwCreateWindow(width, height, name, nullptr, nullptr);


			// Validate if window creation was succesful.
			if (_window == NULL)
			{
				glfwTerminate();
				assert(false, "glfwWindow is NULL");
			}


			// Set the new window to the active window.
			glfwMakeContextCurrent(_window);
		}


		// Returns the GLFWwindow ptr.
		GLFWwindow* getGlfwWindow() const
		{
			return _window;
		}


		// Updates the GLFWwindow .
		void swapBuffer() const
		{
			// Switches the OpenGL frame buffers.
			glfwSwapBuffers(_window);
		}


	private:

		GLFWwindow* _window;
	};

	struct rendererSettings
	{
		bool IsVsync = false;
		glm::vec4 ClearColor = { 0, 0, 0, 1 };
	};



	class renderer
	{
	public:

		renderer(const window& window, const rendererSettings& settings)
			: _isInitialized(false), _window(window), _renderSettings(settings), _shaderProgram(0), _activeTexture(0)
		{}


		class shader
		{
		public:

			shader(const char* vertexSrc = nullptr, const char* fragmentSrc = nullptr)
				: _id(0)
			{

				// Compile shader if src is given.
				if (!(vertexSrc == nullptr && fragmentSrc == nullptr))
				{
					compileShader(vertexSrc, fragmentSrc);
				}

			}

			~shader()
			{
				// Delete shader.
				glDeleteProgram(_id);
			}

			void compileShader(const char* vertexSrc, const char* fragmentSrc)
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

			void bind() const
			{
				assert(!(_id == 0), "Shader is not compiled.");

				// Bind
				glUseProgram(_id);
			}

			void unbind()
			{
				// Unbind shader.
				glUseProgram(0);
			}

		private:

			uint32_t _id;
		};

		class texture
		{
		public:

			texture(const char* filename)
			{

				// Create texture.
				glCreateTextures(GL_TEXTURE_2D, 1, &_id);


				// Set texture if filename exists
				if (filename)
				{
					setTexture(filename);
				}
			}

			~texture()
			{
				// Delete texture.
				glDeleteTextures(1, &_id);
			}

			void setTexture(const char* filename)
			{

				// Load image.
				int width, height, nrChannels;
				stbi_set_flip_vertically_on_load(1);
				unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

				assert(!(data == NULL), "Texture file not found!");

				bind();

				// Set texture filter settings.
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				// Print path the image was found in.
				DEBUG_ONLY(std::cout << "Texture found:  " << std::filesystem::current_path() << "\\" << filename << "\n");

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);


				// Free image data.
				stbi_image_free(data);
			}

			void bind() const
			{
				// Bind texture.
				glBindTexture(GL_TEXTURE_2D, _id);
			}

			void unbind() const
			{
				// Unbind texture.
				glBindTexture(GL_TEXTURE_2D, 0);
			}

		private:
			
			uint32_t _id;
		};

		class vertexBuffer
		{
		public:
			vertexBuffer(const float* vertices, uint32_t elements) 
				: _vboId(0), _vaoId(0)
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

			~vertexBuffer()
			{
				// Delete buffer and vertex array
				glDeleteBuffers(1, &_vboId);
				glDeleteVertexArrays(1, &_vaoId);
			}

			void setVertexData(const float* vertices, uint32_t elements) const
			{

				// Bind VAO, then bind VBO.
				glBindBuffer(GL_ARRAY_BUFFER, _vboId);
				bind();


				// Copy the vertex data into the buffer's memory
				glBufferData(GL_ARRAY_BUFFER, elements * sizeof(float), vertices, GL_STATIC_DRAW);

			}

			// Binds the vertex array.
			void bind() const
			{
				// Bind vao
				glBindVertexArray(_vaoId);
			}

			void unbind() const
			{
				// Unbind buffer.
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

		private:

			uint32_t _vboId;
			uint32_t _vaoId;
		};

		class elementBuffer
		{
		public:
			elementBuffer(const uint32_t* indices, uint32_t elements)
				: _id(0)
			{

				// Generate buffer.
				glGenBuffers(1, &_id);

				// Set vertex data if used as a parameter.
				if (indices)
				{
					setElementData(indices, elements);
				}

			}

			~elementBuffer()
			{
				// Delete buffer.
				glDeleteBuffers(1, &_id);
			}

			void setElementData(const uint32_t* indices, uint32_t elements) const
			{

				// Bind EBO
				bind();

				// Copy the index data into the buffer's memory
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(uint32_t), indices, GL_STATIC_DRAW);

			}

			// Binds the element array.
			void bind() const
			{
				// Bind vao
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
			}

			void unbind() const
			{
				// Bind vao
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}

		private:

			uint32_t _id;
		};


		// Initialize the renderer
		void init()
		{

			// Initialize GLAD.
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				assert(false,"Failed to initialize GLAD");
			}


			// Get window width and height.
			int width = 0;
			int height = 0;
			glfwGetWindowSize(_window.getGlfwWindow(), &width, &height);

			assert(!(width <= 0 || height <= 0), "Window size is 0.");


			// Set viewport to window size.
			glViewport(0, 0, width, height);

			_isInitialized = true;

		}

		uint32_t glDatatypeSize(uint32_t type) const
		{

			// Return size in bytes from Opengl datatype.
			switch (type)
			{
			case GL_FLOAT:  return 4;
			case GL_INT:    return 4;
			case GL_DOUBLE: return 8;
			default: assert(false, "Datatype not implemented.");
			}

		}

		template<int S>
		struct vertexLayout
		{
			std::array<uint32_t, S> Amount = { 0 };
			std::array<uint32_t, S> DataType = { 0 };

			uint32_t Stride = 0;
		};

		template <int S>
		void setVertexLayout(const vertexLayout<S>& layout) const
		{

			uint32_t offset = 0;

			// Set stride and vertex layout.
			for (size_t i = 0; i < S; i++)
			{
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, layout.Amount[i], layout.DataType[i], GL_FALSE, layout.Stride, (const void*)offset);

				offset += glDatatypeSize(layout.DataType[i]) * layout.Amount[i];
			}

		}

		void clear() const
		{

			assert(_isInitialized, "Renderer is not initialized.");

			// Clear buffer data.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Set background color
			glClearColor(_renderSettings.ClearColor.r,
						 _renderSettings.ClearColor.g, 
						 _renderSettings.ClearColor.b, 
						 _renderSettings.ClearColor.a);

		}

		void draw() const
		{
			assert(_isInitialized, "Renderer is not initialized.");

			// Draw Triangles
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		const window* getWindow() const
		{
			return &_window;
		}


	private:
		bool _isInitialized;

		window _window;
		rendererSettings _renderSettings;

		uint32_t _shaderProgram;
		uint32_t _activeTexture;
	};
}