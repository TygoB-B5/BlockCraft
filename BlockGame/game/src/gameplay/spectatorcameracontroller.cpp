#include "spectatorcameracontroller.h"

namespace blockcraft
{

	void spectatorCameraController::update(float deltaTime)
	{

		// Update WASD QE movement.
		float speed = _cameraTranslationSpeed * _cameraSpeedMultiplier;

		if (_input->isKeyHeld(KEY_LEFT_SHIFT))
			speed = _cameraTranslationBoostedSpeed * _cameraSpeedMultiplier;

		if (_input->isKeyHeld(KEY_W))
			_cameraPosition += getForward() * speed * deltaTime;

		if (_input->isKeyHeld(KEY_S))
			_cameraPosition += -getForward() * speed * deltaTime;

		if (_input->isKeyHeld(KEY_D))
			_cameraPosition += -getRight() * speed * deltaTime;

		if (_input->isKeyHeld(KEY_A))
			_cameraPosition += getRight() * speed * deltaTime;

		if (_input->isKeyHeld(KEY_Q))
			_cameraPosition += getUp() * speed * deltaTime;

		if (_input->isKeyHeld(KEY_E))
			_cameraPosition += -getUp() * speed * deltaTime;


		// Update mouse movement.
		glm::vec2 newMousePos = { _input->getMouseY(), _input->getMouseX() };

		if (_input->isMouseButtonHeld(1))
		{
			glm::vec2 acceleration = (_mouseAcceleration - newMousePos) * _cameraSensitivity;
			_cameraRotation += glm::vec3(acceleration.x, acceleration.y, 0);

		}
		_mouseAcceleration = newMousePos;


		// Update camera speed.
		_cameraSpeedMultiplier += _input->isScrollingDown() ? -0.1f: 0 + 
								  _input->isScrollingUp() ? 0.1f : 0;

		_cameraSpeedMultiplier = _cameraSpeedMultiplier < 0 ? 0 : _cameraSpeedMultiplier;


		// Set camera position and rotation.
		_camera.setPosition(_cameraPosition);
		_camera.setRotation(_cameraRotation);

	}

	const glm::vec3& spectatorCameraController::getForward()
	{
		auto& a = _camera.getViewMatrix();
		return  glm::normalize(-glm::vec3(a[0].z, a[1].z, a[2].z));
	}

	const glm::vec3& spectatorCameraController::getRight()
	{
		auto& a = _camera.getViewMatrix();
		return glm::normalize(-glm::vec3(a[0].x, a[1].x, a[2].x));
	}

	const glm::vec3& spectatorCameraController::getUp()
	{
		auto& a = _camera.getViewMatrix();
		return glm::normalize(-glm::vec3(a[0].y, a[1].y, a[2].y));
	}

}