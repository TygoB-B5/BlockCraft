#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace glr
{

	perspectiveCamera::perspectiveCamera(float fov, float aspectRatio, float zNear, float zFar)
		: _projectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar)), _viewMatrix(glm::mat4(1.0f)), _position(glm::vec3(0)), _rotation(glm::vec3(0))
	{
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}

	void perspectiveCamera::setProjection(float fov, float aspectRatio, float zNear, float zFar)
	{

		// Calculate and set new perspective projection.
		_projectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;

	}

	void perspectiveCamera::recalculateViewMatrix()
	{

		// Calculate viewmatrix.
		glm::mat4 transform = glm::translate(glm::mat4(0.1f), _position);
		glm::mat4 rotatex = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0, 0, 1));

		// Calculate inverse.
		_viewMatrix = glm::inverse((transform * rotatey) * rotatex * rotatez);

		// Calculate view projection matrix.
		_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
	}

}

