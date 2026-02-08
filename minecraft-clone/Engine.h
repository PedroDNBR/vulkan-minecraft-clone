#pragma once
#include "VulkanRenderer.h"
#include "TerrainGenerator.h"

class Engine
{
public:
	void start();
	void update();
	void cleanup();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

private:
	std::unique_ptr<VulkanRenderer> renderer;
	std::unique_ptr<TerrainGenerator> terrainGenerator;

	float lastMouseX = 400;
	float lastMouseY = 300;
	float mouseSensitivity = 0.1f;
	bool firstMouse = true;
	bool onFocus = true;

	void handleCursor();
	void setupGlfwMouseInput();
	void handleMouseInput();

	void handleCameraMovement();
};

