#pragma once
#include "engine.h"
#include "rendering/world.h"
#include "gameplay/spectatorcameracontroller.h"

namespace blockcraft
{

	class game
	{
	public:
		game()
			: _running(true)
		{}
		

		void init();
		void update();

		bool getRunning() const {return _running; }

	private:

		bool _running;

		glr::window* _window;
		glr::renderer* _renderer;
		glr::input* _input;
		world* _world;
		blockcraft::spectatorCameraController* _controller;

	};
}