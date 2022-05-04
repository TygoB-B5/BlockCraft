#include "time.h"

#include "GLFW/glfw3.h"

namespace glr
{

	time::time()
		: _deltaTime(0), _elapsedTime(0), _oldElapsedTime(0)
	{}

	void time::update()
	{

		// Get current time.
		_elapsedTime = glfwGetTime();


		// Update deltaTime;
		_deltaTime = _elapsedTime - _oldElapsedTime;



		// Sync old and new elapsed timef.
		_oldElapsedTime = _elapsedTime;
	}

}