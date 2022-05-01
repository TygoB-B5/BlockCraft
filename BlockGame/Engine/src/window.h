#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <assert.h>

namespace glr
{

	class window
	{

	public:

		window(uint32_t width, uint32_t height, const char* name, bool isVsync = true);

		// Returns the GLFWwindow ptr.
		GLFWwindow* getGlfwWindow() const { return _window; }

		// Returns window width.
		uint32_t getWidth() const { return _width; }

		// Returns window height.
		uint32_t getHeight() const { return _height; }


		// Updates the GLFWwindow.
		void update() const;

		void setVsync(bool enabled) const;

	private:

		GLFWwindow* _window;

		uint32_t _width;
		uint32_t _height;
	};

}