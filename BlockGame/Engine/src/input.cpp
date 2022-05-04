#include "input.h"

#include "GLFW/glfw3.h"

namespace glr
{
	/// Keyboard

	input::input(const window* window)
		: _window(window), _scrollingDelta(0), _oldScrollingDelta(0), _isScrollingUp(false), _isScrollingDown(false)
	{
		_setScrollCallback(this, window->getGlfwWindow());
	}

	void input::clear()
	{
		_isScrollingDown = false;
		_isScrollingUp = false;
	}

	bool input::isKeyPressed(int keycode)
	{
		isKeyReleased(keycode);

		// Get key pressed
		auto state = glfwGetKey(_window->getGlfwWindow(), keycode);


		// If key is not pressed return false.
		if (!(state == GLFW_PRESS))
		{
			return false;
		}


		// Return false if key has already been pressed.
		for (uint32_t& key : _heldButtons)
		{
			if (keycode == key)
			{
				return false;
			}
		}


		// Add key to held list.
		_heldButtons.push_back(keycode);

		return true;
	}

	bool input::isKeyHeld(int keycode)
	{

		// Get key pressed or held
		auto state = glfwGetKey(_window->getGlfwWindow(), keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input::isKeyReleased(int keycode)
	{


		// Return false if the key is held.
		if (isKeyHeld(keycode))
		{
			return false;
		}


		// Get key released
		auto state = glfwGetKey(_window->getGlfwWindow(), keycode);


		// Delete key from heldKeys array and return true if the key is released.
		for (size_t i = 0; i < _heldButtons.size(); i++)
		{
			if (keycode == _heldButtons[i])
			{
				// Delete element from _heldButtons array at i.
				_heldButtons.erase(_heldButtons.begin() + i);

				return true;
			}
		}

		return false;
	}


	/// Mouse

	bool input::isMouseButtonPressed(int button)
	{

		// Get key pressed
		auto state = glfwGetMouseButton(_window->getGlfwWindow(), button);


		// If key is not pressed return false.
		if (!(state == GLFW_PRESS))
		{
			return false;
		}


		// Return false if key has already been pressed.
		for (uint32_t& key : _heldButtons)
		{
			if (button == key)
			{
				return false;
			}
		}


		// Add key to held list.
		_heldButtons.push_back(button);

		return true;
	}

	bool input::isMouseButtonHeld(int button)
	{

		// Get key pressed or held
		auto state = glfwGetMouseButton(_window->getGlfwWindow(), button);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input::isMouseButtonReleased(int button)
	{

		// Press mouse button if it not pressed yet. (required to update state if isMouseButtonPressed isnt called from your application)
		isMouseButtonPressed(button);


		// Return false if the key is held.
		if (isMouseButtonHeld(button))
		{
			return false;
		}


		// Get key released
		auto state = glfwGetKey(_window->getGlfwWindow(), button);


		// Delete key from heldKeys array and return true if the key is released.
		for (size_t i = 0; i < _heldButtons.size(); i++)
		{
			if (button == _heldButtons[i])
			{
				// Delete element from _heldButtons array at i.
				_heldButtons.erase(_heldButtons.begin() + i);

				return true;
			}
		}

		return false;
	}

	void input::updateScrollingState(double xoffset, double yoffset)
	{
		_isScrollingDown = yoffset < 0;
		_isScrollingUp = yoffset > 0;
	}

	bool input::isScrollingUp()
	{
		return _isScrollingUp;
	}

	bool input::isScrollingDown()
	{
		return _isScrollingDown;
	}

	/// Mouse position

	std::pair<float, float> input::getMousePosition() const
	{

		// Get mouse position.
		double xPos, yPos;
		glfwGetCursorPos(_window->getGlfwWindow(), &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}



	void _setScrollCallback(input* inp, GLFWwindow* window)
	{
		__inputPtrs.push_back(inp);

		glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				for (auto& ptr : __inputPtrs)
					ptr->updateScrollingState(xoffset, yoffset);
			});
	}
}