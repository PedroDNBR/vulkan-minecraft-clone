#include "VulkanRenderer.h";
#include "TerrainGenerator.h"

int main() {
	VulkanRenderer renderer;
	TerrainGenerator terrainGenerator;
	try
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		std::vector<Vertex> vertices1;
		std::vector<uint32_t> indices1;

		std::vector<const char*> texturesPaths = { "textures/tilesheet.png" };

		/*vertices1 = {
			{{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.06255f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.06255f, 0.06255f}},
			{{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.06255f}}
		};

		indices1 = {
			 0, 1, 2, 2, 3, 0,
		};

		renderer.feedMesh(
			vertices1,
			indices1,
			texturesPaths
		);*/

		terrainGenerator.LoadChunk(0, -1, -2);

		for (const Chunk& chunk : terrainGenerator.loadedChunks)
		{
			for (uint32_t x = 0; x < terrainGenerator.CHUNK_SIZE; x++)
			{
				for (uint32_t y = 0; y < terrainGenerator.CHUNK_SIZE; y++)
				{
					for (uint32_t z = 0; z < terrainGenerator.CHUNK_SIZE; z++)
					{
						int blockIndex = terrainGenerator.index(x, y, z);
						if (chunk.blocks[blockIndex] == BlockType::AIR)
							continue;

						for (size_t f = 0; f < 6; f++)
						{
							int nx = x + terrainGenerator.faces[f].r;
							int ny = y + terrainGenerator.faces[f].g;
							int nz = z + terrainGenerator.faces[f].b;

							int nIndex = terrainGenerator.index(nx, ny, nz);

							if (nx < 0 || nx >= terrainGenerator.CHUNK_SIZE ||
								ny < 0 || ny >= terrainGenerator.CHUNK_SIZE ||
								nz < 0 || nz >= terrainGenerator.CHUNK_SIZE ||
								chunk.blocks[nIndex] == BlockType::AIR)
							{
								glm::vec3 blockWorldPos =
									glm::vec3(x, y, z) +
									glm::vec3(chunk.chunkCoord) * float(terrainGenerator.CHUNK_SIZE);

								uint32_t startIndex = vertices.size();
								for (size_t v = 0; v < 4; v++)
								{
									glm::vec3 vertexFaces = blockWorldPos + terrainGenerator.facesVertex[f][v] * .5f;
									glm::vec3 colorMultiplier = { 1,1,1 }/* + faces[f]*/;
									BlockDef blockDef = terrainGenerator.blockDef[chunk.blocks[blockIndex]];
									glm::ivec2 tileOffset;

									if (chunk.blocks[blockIndex] == BlockType::GRASS && f == 4)
										colorMultiplier = { 0.35f, 1.0f ,0.1f };

									if (f == 4)
										tileOffset = blockDef.topUV;
									else if (f == 5)
										tileOffset = blockDef.bottomUV;
									else
										tileOffset = blockDef.sidesUV;

									glm::vec2 uvMap = (terrainGenerator.uvMaps[v] + glm::vec2(tileOffset)) * terrainGenerator.tileSize;

									vertices.push_back(
										{ 
											vertexFaces,
											colorMultiplier, 
											uvMap
										}
									);
								}
								indices.push_back(startIndex + 0);
								indices.push_back(startIndex + 1);
								indices.push_back(startIndex + 2);
								indices.push_back(startIndex + 2);
								indices.push_back(startIndex + 3);
								indices.push_back(startIndex + 0);
							}
						}
					}
				}
			}
		}

		renderer.feedMesh(
			vertices,
			indices,
			texturesPaths
		);

		//renderer.run();

		renderer.initWindow();
		renderer.initVulkan();

		glm::vec3 cameraPosition = glm::vec3(0.0, 0.0, 0.0);
		glm::vec2 cameraRotation = { 0.0f, 0.0f };

		float deltaTime = 0.0f; // Time between current frame and the last frame
		float lastFrame = 0.0f; // Time of last frame

		glm::vec3 front;
		glm::vec3 right;
		while (!glfwWindowShouldClose(renderer.getWindow()))
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			float speed = 4;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				speed = 8;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
				speed = 4;

			front.x = cos(glm::radians(cameraRotation.x - 90)) * cos(glm::radians(cameraRotation.y));
			front.y = sin(glm::radians(cameraRotation.y));
			front.z = sin(glm::radians(cameraRotation.x - 90)) * cos(glm::radians(cameraRotation.y));

			right = normalize(cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
				cameraPosition += front * speed * deltaTime;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
				cameraPosition -= front * speed * deltaTime;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
				cameraPosition -= right * speed * deltaTime;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
				cameraPosition += right * speed * deltaTime;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
				cameraPosition.y += (speed * deltaTime);

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
				cameraPosition.y -= (speed * deltaTime);

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_I) == GLFW_PRESS)
				cameraRotation.y += (speed * 10 * deltaTime);

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_K) == GLFW_PRESS)
				cameraRotation.y -= (speed * 10 * deltaTime);;

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_J) == GLFW_PRESS)
				cameraRotation.x -= (speed * 10 * deltaTime);

			if (glfwGetKey(renderer.getWindow(), GLFW_KEY_L) == GLFW_PRESS)
				cameraRotation.x += (speed * 10 * deltaTime);

			renderer.updateCameraFront(front);
			renderer.updateCameraPosition(cameraPosition);
			renderer.updateCameraRotation(cameraRotation);
			renderer.mainLoop(deltaTime);
		}
		renderer.finishMainLoop();
		renderer.cleanup();

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}




/*
terrainGenerator.LoadChunk(0,0,0);

for (const Chunk& chunk : terrainGenerator.loadedChunks)
{
	for (uint32_t x = 0; x < terrainGenerator.CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < terrainGenerator.CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < terrainGenerator.CHUNK_SIZE; z++)
			{
				int idx = terrainGenerator.index(x, y, z);
				BlockType block = chunk.blocks[idx];

				if (block == BlockType::AIR) continue;

				if (y < terrainGenerator.CHUNK_SIZE - 1 && chunk.blocks[terrainGenerator.index(x, y + 1, z)] != BlockType::AIR)
					continue;

				uint32_t baseIndex = vertices.size();

				float wx = x + chunk.chunkCoord.x * terrainGenerator.CHUNK_SIZE;
				float wy = y + chunk.chunkCoord.y * terrainGenerator.CHUNK_SIZE;
				float wz = z + chunk.chunkCoord.z * terrainGenerator.CHUNK_SIZE;

				vertices.push_back({ {wx - 1, wy + 1, wz - 1}, {1,1,1}, {0,0} });
				vertices.push_back({ {wx + 1, wy + 1, wz - 1}, {1,1,1}, {0,0} });
				vertices.push_back({ {wx + 1, wy + 1, wz + 1}, {1,1,1}, {0,0} });
				vertices.push_back({ {wx - 1, wy + 1, wz + 1}, {1,1,1}, {0,0} });

				indices.push_back(baseIndex + 0);
				indices.push_back(baseIndex + 1);
				indices.push_back(baseIndex + 2);
				indices.push_back(baseIndex + 3);
				indices.push_back(baseIndex + 0);
			}
		}
	}
}*/

/*vertices = {
	{{-4, 0, -4}, {1,1,1}, {0,0}},
	{{ 4, 0, -4}, {1,1,1}, {1,0}},
	{{ 4, 0,  4}, {1,1,1}, {1,1}},
	{{-4, 0,  4}, {1,1,1}, {0,1}},
};

indices = { 0,1,2,2,3,0 };*/