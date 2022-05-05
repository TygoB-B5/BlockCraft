#pragma once
#include "engine.h"
#include "rendering/world.h"
#include "gameplay/spectatorcameracontroller.h"

namespace blockcraft
{

	class game
	{
	public:

		game();
		~game();

		void update();

		bool getRunning() const {return _running; }

	private:

		bool _running;

		glr::renderer* _renderer;
		world* _world;
		blockcraft::spectatorCameraController* _controller;
	};
}