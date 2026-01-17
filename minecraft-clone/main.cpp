#include "VulkanRenderer.h";
#include "TerrainGenerator.h"

int main() {
	VulkanRenderer renderer;
	TerrainGenerator terrainGenerator;
	try
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		vertices = {
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};

		indices = {
			 0, 1, 2, 2, 3, 0,
		};

		std::vector<const char*> texturesPaths = { "textures/tilesheet.png" };
		renderer.feedMesh(
			vertices,
			indices,
			texturesPaths
		);

		renderer.run();
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