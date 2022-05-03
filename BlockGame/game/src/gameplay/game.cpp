#include "game.h"

namespace blockcraft
{

	game::game()
		: _running(true)
	{
		// Create window.
		_window = new glr::window(1000, 1000, "My window", true);


		// Create rendersettings.
		glr::renderer::rendererSettings settings = glr::renderer::rendererSettings();
		settings.CullingMode = glr::renderer::rendererSettings::cullingMode::Back;
		settings.ClearColor = { 0.5f, 0.5f, 1.0f, 1.0f };
		settings.DepthTestingMode = glr::renderer::rendererSettings::depthTestingMode::Less;


		// Create renderer object.
		_renderer = new glr::renderer(_window, settings);
		_input = new glr::input(_window);


		// Create world and generate chunks.
		_world = new world();

		for (size_t x = 0; x < 1; x++)
		{
			for (size_t z = 0; z < 1; z++)
			{
				_world->addChunk({ x, z });
			}
		}


		// Create spectator controller.
		_controller = new spectatorCameraController(_input, 100, 1 / 1, 0.001f, 10000);

	}

	void game::update()
	{
		// Game loop.
		_renderer->clear();

		_controller->update(0.01);
		_world->draw(&_controller->getCamera(), _renderer);

		_input->clear();
		_window->update();

		_running = !glfwWindowShouldClose(_renderer->getWindow()->getGlfwWindow());
	}

}