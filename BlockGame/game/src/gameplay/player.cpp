#include "player.h"

namespace blockcraft
{

	void player::update(float deltaTime)
	{
		glm::vec3 oldPos = _position;

		if (!isGrounded())
			_position.y -= 9 * deltaTime;


		if (_input->isKeyHeld(KEY_W))
			_position += glm::normalize(glm::vec3(getForward().x, 0, getForward().z)) * WALK_SPEED* deltaTime;

		if (_input->isKeyHeld(KEY_S))
			_position += -glm::normalize(glm::vec3(getForward().x, 0, getForward().z)) * WALK_SPEED * deltaTime;

		if (_input->isKeyHeld(KEY_D))
			_position += -glm::vec3(getRight().x, 0, getRight().z) * WALK_SPEED * deltaTime;

		if (_input->isKeyHeld(KEY_A))
			_position += glm::vec3(getRight().x, 0, getRight().z) * WALK_SPEED * deltaTime;


		// Update mouse movement.
		glm::vec2 newMousePos = { _input->getMouseY(), _input->getMouseX() };
		glm::vec2 acceleration = (_mouseAcceleration - newMousePos) * _sensitivity;
		_rotation += glm::vec3(acceleration.x, acceleration.y, 0);
		_mouseAcceleration = newMousePos;
		_camera.setPosition(_position);
		_camera.setRotation(_rotation);
	}

	bool player::isGrounded()
	{
		bool a = isSolidBlock(_position.x - 0.5f, _position.y - PLAYER_HEIGHT, _position.z - 0.5f);
		if (a) _world->setBlock(_position.x - 0.5f, _position.y - PLAYER_HEIGHT, _position.z - 0.5f, 3);
		return a;
	}

	bool player::isSolidBlock(int32_t x, uint8_t y, int32_t z)
	{
		return !blockData::isTransparent(_world->getBlock(x, y, z));
	}

	const glm::vec3& player::getForward()
	{
		auto& a = _camera.getViewMatrix();
		return  glm::normalize(-glm::vec3(a[0].z, a[1].z, a[2].z));
	}

	const glm::vec3& player::getRight()
	{
		auto& a = _camera.getViewMatrix();
		return glm::normalize(-glm::vec3(a[0].x, a[1].x, a[2].x));
	}

	const glm::vec3& player::getUp()
	{
		auto& a = _camera.getViewMatrix();
		return glm::normalize(-glm::vec3(a[0].y, a[1].y, a[2].y));
	}

}