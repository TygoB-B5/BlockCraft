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
		_controller = new spectatorCameraController(_renderer->getInput(), 100, 16.0f/9.0f, 0.0001f, 10000.0f);
		_player = new player(_renderer->getInput(), _world);
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
		_player->update(_renderer->getTime().getDeltaTime());

		// Temporary chunk rendering thingie.
		
		for (int x = -64; x < 64; x++)
		{
			for (int z = -64; z < 64; z++)
			{
				glm::vec2 pos = { x * CHUNK_SIZE, z * CHUNK_SIZE };
				bool inRange = glm::distance(pos, { _player->getCamera().getPosition().x, _player->getCamera().getPosition().z }) < 128;
				 
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

		GLR_LOG(_controller->getCamera().getPosition().x << " - " << _controller->getCamera().getPosition().y << " - " << _controller->getCamera().getPosition().z)
		
		if (_renderer->getInput()->isKeyHeld(KEY_SPACE))
		{
			int x = _controller->getCamera().getPosition().x;
			int y = _controller->getCamera().getPosition().y;
			int z = _controller->getCamera().getPosition().z;

			_world->setBlock(x, y, z, 1);	
		}



		_world->draw(&_player->getCamera(), _renderer);

		_renderer->update();

		_running = !glfwWindowShouldClose(_renderer->getWindow().getGlfwWindow());
	}

}