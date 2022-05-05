#pragma once

#include "core.h"
#include "GLFW/glfw3.h"

#ifdef GLR_DEBUG
	#define GLR_PROFILE_START glr::profiler p;
	#define GLR_PROFILE_GET(message) GLR_LOG(message << p.stop())
#else
	#define GLR_PROFILE_START
	#define GLR_PROFILE_GET(message)
#endif

namespace glr
{

	class profiler
	{
	public:

		profiler()
		{
			_startTime = (float)glfwGetTime();
		}

		float stop()
		{
			float startTime = _startTime;
			_endTime = (float)glfwGetTime();
			_startTime = _endTime;
			return (_endTime - startTime) * 1000;
		}


	private:

		float _startTime;
		float _endTime;
	};
}