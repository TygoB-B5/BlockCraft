#include "engine.h"
#include "glm/gtc/matrix_transform.hpp"
#include "rendering/camera/spectatorcameracontroller.h"
#include "rendering/world.h"


int main()
{

	// Create window.
	auto window = glr::window(1000, 1000, "My window", true);


	// Create rendersettings.
	auto settings = glr::renderer::rendererSettings();
	settings.CullingMode = glr::renderer::rendererSettings::cullingMode::Back;
	settings.ClearColor = { 0.5f, 0.5f, 1.0f, 1.0f };
	settings.DepthTestingMode = glr::renderer::rendererSettings::depthTestingMode::Less;


	// Create renderer object.
	glr::renderer renderer(&window, settings);
	glr::input input(&window);


	// Create world and generate chunks.
	blockgame::world world;

	for (size_t x = 0; x < 16; x++)
	{
		for (size_t z = 0; z < 16; z++)
		{
			world.addChunk({ x, z });
		}
	}


	// Create spectator controller.
	blockgame::spectatorCameraController cont(&input, 100, 1 / 1, 0.001f, 10000);


	// Game loop.
	while (!glfwWindowShouldClose(renderer.getWindow()->getGlfwWindow()))
	{
		renderer.clear();

		cont.update(0.01);
		world.draw(&cont.getCamera(), &renderer);

		input.clear();
		window.update();
	}

	glfwTerminate();
	return 0;
}