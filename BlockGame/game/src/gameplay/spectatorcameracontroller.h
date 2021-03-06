#pragma once

#include "engine.h"

namespace blockcraft
{
	class spectatorCameraController
	{
	public:

		spectatorCameraController(glr::input* input, float fov, float aspectRatio, float zNear, float zFar)
			: _aspectRatio(aspectRatio), _fov(fov), _near(zNear), _far(zFar), _camera(_fov, _aspectRatio, _near, _far), _input(input)
		{}

		void update(float deltaTime);

		void setPosition(const glm::vec3& position) { _cameraPosition = position; }
		void setRotation(const glm::vec3& rotation) { _cameraRotation= rotation; }

		const glm::vec3& getForward();
		const glm::vec3& getRight();
		const glm::vec3& getUp();
		const glr::perspectiveCamera& getCamera() { return _camera; }

	private:

		float _aspectRatio;
		float _fov;
		float _near;
		float _far;

		glr::perspectiveCamera _camera;
		glr::input* _input;

		glm::vec3 _cameraPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _cameraRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec2 _mouseAcceleration = { 0.0f, 0.0f };
		float _cameraSpeedMultiplier = 1.0f;
		float _cameraTranslationSpeed = 10.0f;
		float _cameraTranslationBoostedSpeed = 30.0f;
		float _cameraSensitivity = 0.33f;
	};
}