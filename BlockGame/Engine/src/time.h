#pragma once

#include <chrono>

namespace glr
{

	class time
	{
	public:

		time();

		void update();

		float getDeltaTime() const { return _deltaTime; }
		float getElapsedTime() const { return _elapsedTime; }
		uint32_t getFPS() const { return 1 / _deltaTime; }


	private:

		float _elapsedTime, _oldElapsedTime;
		float _deltaTime;
	};

}