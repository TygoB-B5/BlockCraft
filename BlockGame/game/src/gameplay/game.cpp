#include "game.h"

namespace blockcraft
{

	game::game()
		: _running(true)
	{
		// Create rendersettings.
		glr::renderer::rendererSettings settings = glr::renderer::rendererSettings();
		settings.ClearColor = { 0.5f, 0.5f, 1.0f, 1.0f };
		settings.CullingMode = glr::renderer::rendererSettings::cullingMode::Back;
		settings.DepthTestingMode = glr::renderer::rendererSettings::depthTestingMode::Less;

		// Create renderer object.
		_renderer = new glr::renderer(glr::window(1280, 720, "My window", false), settings);


		// Create world and generate chunks.
		_world = new world();

		for (size_t x = 0; x < 16; x++)
		{
			for (size_t z = 0; z < 16; z++)
			{
				_world->addChunk({ x, z });
			}
		}

		// Create spectator controller.
		_controller = new spectatorCameraController(_renderer->getInput(), 100, (float)_renderer->getWindow().getWidth() / (float)_renderer->getWindow().getHeight(), 0.001f, 10000);

	}

	game::~game()
	{
		// Free all objects.
		delete(_renderer);
		delete(_world);
		delete(_controller);
	}


	void game::update()
	{
		// Game loop.
		_renderer->clear();
		_controller->update(_renderer->getTime().getDeltaTime());

		_world->setBlock(rand() % 256, 31, rand() % 256, 1);

		_world->draw(&_controller->getCamera(), _renderer);

		_renderer->update();

		_running = !glfwWindowShouldClose(_renderer->getWindow().getGlfwWindow());
	}

}