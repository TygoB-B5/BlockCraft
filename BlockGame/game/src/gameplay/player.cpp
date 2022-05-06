#include "player.h"

namespace blockcraft
{

	void player::update(float deltaTime)
	{
		
		if (_input->isKeyHeld(KEY_W))
			_cameraPosition += getForward() * deltaTime;

		if (_input->isKeyHeld(KEY_S))
			_cameraPosition += -getForward() * deltaTime;

		if (_input->isKeyHeld(KEY_D))
			_cameraPosition += -getRight() * deltaTime;

		if (_input->isKeyHeld(KEY_A))
			_cameraPosition += getRight() * deltaTime;

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