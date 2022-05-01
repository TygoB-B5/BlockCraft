#include "window.h"

namespace glr
{

	window::window(uint32_t width, uint32_t height, const char* name, bool isVsync)
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


		// Save window width and height.
		_width = width;
		_height = height;


		// Set the new window to the active window.
		glfwMakeContextCurrent(_window);

		setVsync(isVsync);

	}


	// Updates the GLFWwindow .
	void window::update() const
	{
		// Switches the OpenGL frame buffers.
		glfwSwapBuffers(_window);

		// Update input
		glfwPollEvents();
	}

	void window::setVsync(bool enabled) const
	{
		// Enables and disables vsync based on bool.
		glfwSwapInterval(enabled ? 1 : 0);
	}

}