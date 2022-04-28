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

	This requires some stuff tho:
	Data container for Textures, Shaders, VAO, VBO, EBO
	Abstraction for all of those things. (already did Textures and Shaders)

	I also need to deal with the Input bs.

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
		GLFWwindow* get() const
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

			void bindShader()
			{
				assert(!(_id == 0), "Shader is not compiled.");
				glUseProgram(_id);
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


		private:

			uint32_t _id;
		};

		class texture
		{
			// TODO
		};

		class vertex
		{

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
			glfwGetWindowSize(_window.get(), &width, &height);

			assert(!(width <= 0 || height <= 0), "Window size is 0.");


			// Set viewport to window size.
			glViewport(0, 0, width, height);

			_isInitialized = true;

		}

		uint32_t glDatatypeSize(uint32_t type)
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
		void setVertexLayout(const vertexLayout<S>& layout)
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


		void setTexture(uint32_t slot, const char* filename)
		{	

			// Load image.
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(1);
			unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

			assert(!(data == NULL), "Texture file not found!");


			// Print path the image was foudn in
			DEBUG_ONLY(std::cout << "Texture found:  " << std::filesystem::current_path() << "\\" << filename << "\n");


			// Create texture and assign data.
			unsigned int texture;
			glCreateTextures(GL_TEXTURE_2D, 1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			// Set texture filter settings.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);


			// Free image data.
			stbi_image_free(data);

			_activeTexture = texture;

		}

		void bindActiveTexture()
		{
			glBindTexture(GL_TEXTURE_2D, _activeTexture);
		}

		void addVertexElements();

		
		// Clears the frame buffer
		void clear()
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


		void draw()
		{
			assert(_isInitialized, "Renderer is not initialized.");

			// Draw Triangles
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}


	public:
		bool _isInitialized;

		window _window;
		rendererSettings _renderSettings;

		uint32_t _shaderProgram;
		uint32_t _activeTexture;

		std::string _vertexShaderSource;
		std::string _fragmentShaderSource;

	};
}