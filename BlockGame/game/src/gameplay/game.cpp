#include "game.h"

#include <thread>

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
		_world = new world(34535);

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

		GLR_LOG(_renderer->getTime().getFPS())

		for (int x = -128; x < 128; x++)
		{
			for (int z = -128; z < 128; z++)
			{
				glm::vec2 pos = { x * 16, z * 16 };

				if (glm::distance(pos, { _controller->getCamera().getPosition().x, _controller->getCamera().getPosition().z }) < 128 &&
					_world->getChunkFromPosition({ x, z }) == nullptr)
					_world->addChunk({ x, z }); 
			}
		}





		_world->draw(&_controller->getCamera(), _renderer);

		_renderer->update();

		_running = !glfwWindowShouldClose(_renderer->getWindow().getGlfwWindow());
	}

}