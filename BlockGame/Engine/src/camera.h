#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace blockcraft
{
	class spectatorCamera
	{
	public:
		spectatorCamera(float fov, float aspectRatio, float zNear, float zFar);

		void setProjection(float fov, float aspectRatio, float zNear, float zFar);

		void setPosition(const glm::vec3& position) { _position = position; recalculateViewMatrix(); }
		void setRotation(const glm::vec3& rotation) { _rotation = rotation; recalculateViewMatrix(); }

		const glm::vec3& getPosition() const { return _position; }
		const glm::vec3& getRotation() const { return _rotation; }


		const glm::mat4& getViewProjectionMatrix() const { return _viewProjectionMatrix; }
		const glm::mat4& getProjectionMatrix() const { return _projectionMatrix; }
		const glm::mat4& getViewMatrix() const { return _viewMatrix; }

	private:
		void recalculateViewMatrix();

	private:
		glm::mat4 _projectionMatrix;
		glm::mat4 _viewMatrix;
		glm::mat4 _viewProjectionMatrix;

		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
	};
}