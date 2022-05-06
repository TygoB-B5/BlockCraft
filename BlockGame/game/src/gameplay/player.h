#pragma once

#include "engine.h"
#include "../rendering/world.h"

namespace blockcraft
{
	class player
	{
	public:

		player(glr::input* input, world* world)
			: _camera(glm::radians(100.0f), 16.0f/9.0f, 0.001f, 10000.0f), _input(input), _world(world)
		{}

		void update(float deltaTime);

		const glm::vec3& getForward();
		const glm::vec3& getRight();
		const glm::vec3& getUp();

		const glr::perspectiveCamera& getCamera() { return _camera; }

	private:

		bool isGrounded();
		bool isSolidBlock(int32_t x, uint8_t y, int32_t z);


		glr::perspectiveCamera _camera;
		glr::input* _input;

		world* _world;

		glm::vec3 _cameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _cameraRotation = { 0.0f, 0.0f, 0.0f };
	};
}