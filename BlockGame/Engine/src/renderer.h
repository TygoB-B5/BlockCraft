#pragma once




//////////////////////////////////////////////////////////////
/// Simple rendering and input abstraction for GLFW Opengl.///
/// Tygo Boons 2022 <3                                     ///
//////////////////////////////////////////////////////////////





/// Preprocessor defines

#ifdef _DEBUG
#define DEBUG_ONLY(expression) expression
#else
#define DEBUG_ONLY(expression)
#endif


// GLFW Keycodes
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39  /* ' */
#define KEY_COMMA              44  /* , */
#define KEY_MINUS              45  /* - */
#define KEY_PERIOD             46  /* . */
#define KEY_SLASH              47  /* / */
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59  /* ; */
#define KEY_EQUAL              61  /* = */
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91  /* [ */
#define KEY_BACKSLASH          92  /* \ */
#define KEY_RIGHT_BRACKET      93  /* ] */
#define KEY_GRAVE_ACCENT       96  /* ` */
#define KEY_WORLD_1            161 /* non-US #1 */
#define KEY_WORLD_2            162 /* non-US #2 */

/* Funct keys */
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348




/// Code

#include "glad/glad.h"
#include <glfw/glfw3.h>
#include <iostream>
#include <assert.h>
#include <array>
#include <vector>
#include "stb_image.h"
#include "glm/glm.hpp"
#include <filesystem>


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


	class input
	{
	public:
		
		input(const window& window)
			: _window(window)
		{}

		bool isKeyPressed(int keycode)
		{

			// Get key pressed
			auto state = glfwGetKey(_window.getGlfwWindow(), keycode);

			// If key is not pressed return false.
			if (!(state == GLFW_PRESS))
			{
				return false;
			}


			// Return false if key has already been pressed.
			for (uint32_t& key : _heldKeys)
			{
				if (keycode == key)
				{
					return false;
				}
			}


			// Add key to held list.
			_heldKeys.push_back(keycode);

			return true;
		}

		bool isKeyHeld(int keycode)
		{

			// Get key pressed or held
			auto state = glfwGetKey(_window.getGlfwWindow(), keycode);

			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		bool isKeyReleased(int keycode)
		{

			// Return false if the key is held.
			if (isKeyHeld(keycode))
			{
				return false;
			}


			// Get key released
			auto state = glfwGetKey(_window.getGlfwWindow(), keycode);

			// Delete key from heldKeys array and return true if the key is released.
			for (size_t i = 0; i < _heldKeys.size(); i++)
			{
				if (keycode == _heldKeys[i])
				{
					_heldKeys.erase(_heldKeys.begin() + i);

					return true;
				}
			}

			return false;
		}

		bool isMouseButtonPressed(int button)
		{

			// Get Window.
			auto* window = _window.getGlfwWindow();


			// Get mouse button pressed.
			auto state = glfwGetMouseButton(window, button);
			return state == GLFW_PRESS;
		
		}

		float getMouseX()
		{
			return getMousePosition().first;
		}

		float getMouseY()
		{
			return getMousePosition().second;
		}

		std::pair<float, float> getMousePosition()
		{

			// Get Window.
			auto* window = _window.getGlfwWindow();


			// Get mouse position.
			double xPos, yPos;
			glfwGetCursorPos(window, &xPos, &yPos);

			return { (float)xPos, (float)yPos };
		}

	private:
		std::vector<uint32_t> _heldKeys;

		window _window;
	};


	class renderer
	{
	public:

		renderer(const window& window, const rendererSettings& settings)
			: _window(window), _renderSettings(settings)
		{
			init();
		}


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

			vertexBuffer(const std::vector<float>& vertices)
				: _vboId(0), _vaoId(0), _vertexAmount(0)
			{

				// Generate buffer and array.
				glGenVertexArrays(1, &_vaoId);
				glGenBuffers(1, &_vboId);

				// Set vertex data if used as a parameter.
				if (!vertices.empty())
				{
					setVertexData(&vertices[0], vertices.size());
				}

			}

			vertexBuffer(const float* vertices, uint32_t elements) 
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

			~vertexBuffer()
			{
				// Delete buffer and vertex array
				glDeleteBuffers(1, &_vboId);
				glDeleteVertexArrays(1, &_vaoId);
			}

			void setVertexData(const float* vertices, uint32_t elements)
			{

				// Bind VAO, then bind VBO.
				glBindBuffer(GL_ARRAY_BUFFER, _vboId);
				bind();


				// Copy the vertex data into the buffer's memory
				glBufferData(GL_ARRAY_BUFFER, elements * sizeof(float), vertices, GL_STATIC_DRAW);

				_vertexAmount = elements;
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

			uint32_t getVertexAmount() const
			{
				return _vertexAmount;
			}

		private:

			uint32_t _vboId;
			uint32_t _vaoId;
			
			uint32_t _vertexAmount;
		};


		class elementBuffer
		{
		public:

			elementBuffer(const uint32_t* indices, uint32_t elements)
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

			elementBuffer(const std::vector<uint32_t>& indices)
				: _id(0), _elementAmount(0)
			{

				// Generate buffer.
				glGenBuffers(1, &_id);

				// Set vertex data if used as a parameter.
				if (!indices.empty())
				{
					setElementData(&indices[0], indices.size());
				}

			}

			~elementBuffer()
			{
				// Delete buffer.
				glDeleteBuffers(1, &_id);
			}

			void setElementData(const uint32_t* indices, uint32_t elements)
			{

				// Bind EBO
				bind();

				// Copy the index data into the buffer's memory
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(uint32_t), indices, GL_STATIC_DRAW);

				_elementAmount = elements;
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

			uint32_t getElementAmount() const
			{
				return _elementAmount;
			}

		private:

			uint32_t _id;
			uint32_t _elementAmount;
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

		}

		static uint32_t glDatatypeSize(uint32_t type)
		{

			// Return size in bytes from Opengl datatype.
			switch (type)
			{
			case GL_FLOAT:        return 4;
			case GL_INT:          return 4;
			case GL_DOUBLE:       return 8;
			case GL_UNSIGNED_INT: return 4;

			default: assert(false, "Datatype not implemented.");
			}

		}

		template<int S>
		struct vertexLayout
		{
		public:

			void bind() const
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

			std::array<uint32_t, S> Amount = { 0 };
			std::array<uint32_t, S> DataType = { 0 };

			uint32_t Stride = 0;
		};

		struct vertexLayoutDynamic
		{
		public:

			vertexLayoutDynamic(uint32_t initialSize = 0)
			{
				setSize(initialSize);
			}

			void setSize(uint32_t size)
			{
				// Resize vectors to specified size.
				Amount.resize(size);
				DataType.resize(size);
			}

			void bind() const
			{

				uint32_t offset = 0;

				// Set stride and vertex layout.
				for (size_t i = 0; i < Amount.size(); i++)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i, Amount[i], DataType[i], GL_FALSE, Stride, (const void*)offset);

					offset += glDatatypeSize(DataType[i]) * Amount[i];
				}

			}


		public:

			std::vector<uint32_t> Amount = { 0 };
			std::vector<uint32_t> DataType = { 0 };

			uint32_t Stride = 0;
		};


		void clear() const
		{

			// Clear buffer data.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Set background color
			glClearColor(_renderSettings.ClearColor.r,
						 _renderSettings.ClearColor.g, 
						 _renderSettings.ClearColor.b, 
						 _renderSettings.ClearColor.a);

		}

		void draw(uint32_t elements) const
		{
			// Draw Triangles
			glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);
		}

		const window* getWindow() const
		{
			return &_window;
		}


	private:

		window _window;
		rendererSettings _renderSettings;
	};
}