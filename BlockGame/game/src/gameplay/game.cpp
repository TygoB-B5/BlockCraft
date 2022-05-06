#include "game.h"


namespace blockcraft
{

	game::game()
		: _running(true)
	{
		// Create rendersettings.
		glr::renderer::rendererSettings settings = glr::renderer::rendererSettings();
		settings.ClearColor = { 0.65f, 0.75f, 1.0f, 1.0f };
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


		// Temporary chunk rendering thingie.
		
		for (int x = -64; x < 64; x++)
		{
			for (int z = -64; z < 64; z++)
			{
				glm::vec2 pos = { x * CHUNK_SIZE, z * CHUNK_SIZE };
				bool inRange = glm::distance(pos, { _controller->getCamera().getPosition().x, _controller->getCamera().getPosition().z }) < 256;
				 
				if (inRange && !_world->getChunkFromPosition({ x, z }))
				{
					_world->addChunk({ x, z });
				}

				if (!inRange && _world->getChunkFromPosition({ x, z }))
				{
					_world->removeChunk({ x, z });
				}

			}
		}

		GLR_LOG(_controller->getCamera().getPosition().x << " - " << _controller->getCamera().getPosition().z)
		
		if (_renderer->getInput()->isKeyHeld(KEY_SPACE))
		{
			int x = _controller->getCamera().getPosition().x;
			int y = _controller->getCamera().getPosition().y;
			int z = _controller->getCamera().getPosition().z;
			_world->setBlock(x, y, z, ID_BLOCK_GRASS);
		}



		_world->draw(&_controller->getCamera(), _renderer);

		_renderer->update();

		_running = !glfwWindowShouldClose(_renderer->getWindow().getGlfwWindow());
	}

}