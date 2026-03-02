#include "Engine.h"

void Engine::start()
{
	renderer = std::make_unique<VulkanRenderer>();
	terrainGenerator = std::make_unique<TerrainGenerator>();

	renderer->camera = Camera();
	renderer->camera.position = { 0.0f, 90.0f, 0.0f };
	renderer->camera.rotation = { 135.0f, -20.0f };

	std::vector<const char*> texturesPaths = { "textures/tilesheet.png" };
	renderer->uploadMeshTexture(texturesPaths);

	renderer->initWindow();
	renderer->initVulkan();

	const int worldSizeX = 4;
	const int worldSizeY = 4;
	const int worldSizeZ = 4;

	TerrainData terrainData;
	terrainData.seed = 1;
	terrainData.baseHeight = 64;
	terrainData.dirtDepth = 8;
	terrainData.startingCaveDepth = 4;
	terrainData.caveThreshold = 0.75f;
	terrainData.heightRange = 20;
	terrainData.scale = 0.02f;
	terrainData.octaves = 4;
	terrainData.caveScale = 0.035f;
	terrainData.caveSeedOffset = 1.f;

	terrainGenerator->SetTerrainSettings(terrainData);
	terrainGenerator->loadedChunks.reserve(worldSizeX * worldSizeY * worldSizeZ);

	for (int x = 0; x < worldSizeX; x++) {
		for (int z = 0; z < worldSizeZ; z++) {
			for (int y = 0; y < worldSizeY; y++) {
				terrainGenerator->LoadChunk(x,y,z);
			}
		}
	}

	for (size_t i = 0; i < (worldSizeX * worldSizeY * worldSizeZ); i++)
	{
		terrainGenerator->generateChunkMesh(i);
		if (terrainGenerator->loadedChunks[i].cpuMesh->vertices.size() > 0)
			terrainGenerator->loadedChunks[i].gpuMeshIndex = renderer->uploadCpuMesh(*terrainGenerator->loadedChunks[i].cpuMesh);
		else
			terrainGenerator->loadedChunks[i].gpuMeshIndex = -1;
	}

	setupGlfwMouseInput();
}

void Engine::update()
{
	while (!glfwWindowShouldClose(renderer->getWindow()))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleCursor();

		if (onFocus)
		{
			handleCameraMovement();
			handleMouseInput();
		}

		renderer->camera.setCameraForward();
		renderer->camera.setCameraRight();

		renderer->clearDrawMeshList();

		glm::mat4 viewProjection = renderer->getCameraProjection(60.f) * renderer->getCameraView();
		glm::mat4 viewProjTranposed = glm::transpose(viewProjection);

		std::array<glm::vec4, 6> frustumPlanes{
			viewProjTranposed[3] + viewProjTranposed[0],
			viewProjTranposed[3] - viewProjTranposed[0],
			viewProjTranposed[3] + viewProjTranposed[1],
			viewProjTranposed[3] - viewProjTranposed[1],
			viewProjTranposed[3] + viewProjTranposed[2],
			viewProjTranposed[3] - viewProjTranposed[2],
		};

		for (size_t i = 0; i < frustumPlanes.size(); i++)
		{
			glm::vec3 normal = glm::vec3(frustumPlanes[i]);
			float length = glm::length(normal);
			if(length > 0)
				frustumPlanes[i] /= length;
		}

		for (size_t i = 0; i < terrainGenerator->loadedChunks.size(); i++)
		{
			if (terrainGenerator->loadedChunks[i].gpuMeshIndex < 0) continue;

			if (!terrainGenerator->isChunkVisible(terrainGenerator->loadedChunks[i], frustumPlanes)) continue;

			renderer->queueMesh(terrainGenerator->loadedChunks[i].gpuMeshIndex);
		}

		renderer->mainLoop();
	}
}

void Engine::cleanup()
{
	renderer->finishMainLoop();
	renderer->cleanup();

	renderer = nullptr;
	renderer = nullptr;
}

void Engine::handleCursor()
{
	if (onFocus && glfwGetKey(renderer->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetInputMode(renderer->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
		onFocus = false;
	}

	if (!onFocus && glfwGetMouseButton(renderer->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		firstMouse = true;
		onFocus = true;
		glfwSetInputMode(renderer->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Engine::setupGlfwMouseInput()
{
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(renderer->getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSetInputMode(renderer->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::handleMouseInput()
{
	double xPosIn, yPosIn;
	glfwGetCursorPos(renderer->getWindow(), &xPosIn, &yPosIn);

	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (firstMouse)
	{
		lastMouseX = xPos;
		lastMouseY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastMouseX;
	float yOffset = yPos - lastMouseY;
	lastMouseX = xPos;
	lastMouseY = yPos;

	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	renderer->camera.rotation.x += xOffset;
	renderer->camera.rotation.y -= yOffset;
}

void Engine::handleCameraMovement()
{
	float speed = 8;

	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 16;

	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		speed = 8;


	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		renderer->camera.position += renderer->camera.forward * speed * deltaTime;

	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		renderer->camera.position -= renderer->camera.forward * speed * deltaTime;

	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		renderer->camera.position -= renderer->camera.right * speed * deltaTime;

	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		renderer->camera.position += renderer->camera.right * speed * deltaTime;


	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		renderer->camera.position.y += (speed * deltaTime);

	if (glfwGetKey(renderer->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		renderer->camera.position.y -= (speed * deltaTime);
}
