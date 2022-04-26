#pragma once

#include "glad/glad.h"
#include <glfw/glfw3.h>
#include "glm/glm.hpp"
#include <iostream>
#include <assert.h>
#include <string>


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



	class input
	{
	public:
		bool isPressed(char key);
		bool isReleased(char key);
		bool isHeld(char key);

	private:
		window* _window;
	};




	enum shaderType
	{
		Vertex = 0,
		Fragment = 1
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
			: _isInitialized(false), _window(window), _renderSettings(settings), _shaderProgram(0)
		{
		}


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


		void setStride();
		void setActiveTexture(uint32_t slot, void* data);

		void addVertexElements();

		void setShader(const char* vertexSrc, const char* fragmentSrc)
		{
			compileShader(vertexSrc, fragmentSrc);

			_vertexShaderSource   = vertexSrc;
			_fragmentShaderSource = fragmentSrc;
		}



		void compileShader(const char* vertexSrc, const char* fragmentSrc)
		{
			// Create a vertex shader object
			unsigned int vertexShader;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);

			// Attach the shader source code to the shader object
			glShaderSource(vertexShader, 1, &vertexSrc, NULL);

			// Compile the vertex shader dynamically 
			glCompileShader(vertexShader);



			// Check if compilation was successful
			GLint success = false;
			char infoLog[512];

			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
				std::cout << infoLog << std::endl;
			}


			// Create a fragment shader object
			unsigned int fragmentShader;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


			// Attach the shader source code to the shader object
			glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);


			// Compile the fragment shader dynamically
			glCompileShader(fragmentShader);
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
				std::cout << infoLog << std::endl;
			}


			// Create a shader program
			unsigned int shaderProgram;
			shaderProgram = glCreateProgram();


			// Attach the compiled shaders to the shader program
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			// Check if linking was successful
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
				std::cout << "ERROR::PROGRAM::LINKING_FAILED\n";
				std::cout << infoLog << std::endl;
			}


			// Assign shaderProgram to renderer.
			_shaderProgram = shaderProgram;


			// Delete shader objects if we no longer need them anymore
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

		}


		// Clears the frame buffer
		void clear()
		{

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
			// Use saved shader
			glUseProgram(_shaderProgram);

			// Draw Triangles
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}


	public:
		bool _isInitialized;

		window _window;
		rendererSettings _renderSettings;

		uint32_t _shaderProgram;

		std::string _vertexShaderSource;
		std::string _fragmentShaderSource;

	};
}