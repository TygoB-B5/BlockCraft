#pragma once
#include "engine.h"
#include "world/world.h"
#include "player.h"
#include "spectatorcameracontroller.h"


	////////////////////////////////////////////////////////////
	// NOTE:                                                  //
	//           BlockCraft.                                  //
	//           Made by Tygo Boons 2022. <3                  //
	//                                                        //
	//           Libs:                                        //
	//           OpenGL, Glad, GLFW, GLM and stb_image.       //
	//                                                        //
	////////////////////////////////////////////////////////////




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

		blockcraft::player* _player;

		// Temporary camera controller for debugging purposes.
		blockcraft::spectatorCameraController* _controller;

	};
}