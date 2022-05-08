#pragma once

#include "engine.h"
#include "world/world.h"

#define PLAYER_HEIGHT 1.5f
#define WALK_SPEED 4.0f

namespace blockcraft
{
	class player
	{
	public:

		player(glr::input* input, world* world)
			: _camera(100.0f, 16.0f/9.0f, 0.001f, 10000.0f), _input(input), _world(world)
		{}

		void update(float deltaTime);

		const glm::vec3& getForward();
		const glm::vec3& getRight();
		const glm::vec3& getUp();

		const glr::perspectiveCamera& getCamera() { return _camera; }

	private:

		void positionToBlockCoord(float* x, float* y, float* z);

		bool isGrounded();
		bool isSolidBlock(int32_t x, uint8_t y, int32_t z);

	private:

		glr::perspectiveCamera _camera;
		glr::input* _input;

		world* _world;

		float _sensitivity = 0.5f;
		glm::vec2 _mouseAcceleration = { 0.0f, 0.0f };

		glm::vec3 _position = { 0.0f, 20.0f, 0.0f };
		glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
	};
}