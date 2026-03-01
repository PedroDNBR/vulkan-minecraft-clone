#include <vector>
#include "Chunk.h"
#include "BlockDef.h"
#include "TerrainData.h"
#include "CpuMesh.h"

class TerrainGenerator
{
public:
	// TerrainGenerator(int32_t seed, int32_t baseHeight, int32_t heightAmplification, float scale, uint8_t octaves, float lacunarity, float persistence);
	void SetTerrainSettings(TerrainData newTerrainData);
	void LoadChunk(int32_t x, int32_t y, int32_t z);
	void SampleHeight(int worldX, int worldZ, int& height);
	void CarveCaves(int worldY, int height, Chunk& chunk, int blockIndex, int worldX, int worldZ);
	std::vector<Chunk> loadedChunks;
	bool isBlockSolid(const Chunk& chunk, int x, int y, int z);
	void generateChunkMesh(int32_t chunkIndex);

	int32_t seed;

	inline int index(int x, int y, int z) {
		return x + Chunk::CHUNK_SIZE * (z + Chunk::CHUNK_SIZE * y);
	}

	const glm::vec2 tileSize = { 1.0 / 16.0, 1.0 / 16.0 };

	const BlockDef blockDef[BlockType::MAX] = {
		{{0,0},{0,0},{0,0}},
		{{0,0},{3,0},{2,0}},
		{{2,0},{2,0},{2,0}},
		{{1,0},{1,0},{1,0}}
	};

	const glm::vec3 faces[6] = {
		 { 0,  0,  1 }, // front
		 { 0,  0, -1 }, // back
		 {-1,  0,  0 }, // left
		 { 1,  0,  0 }, // right
		 { 0,  1,  0 }, // top
		 { 0, -1,  0 }, // bottom
	};

	/*const glm::vec2 uvMaps[4] = {
		{ 0.0f, 0.0f },
		{ 0.06255f, 0.0f },
		{ 0.06255f, 0.06255f },
		{ 0.0f, 0.06255f }
	};*/

	const glm::vec2 uvMaps[4] = {
		{ 0.0f, 1.0f },
		{ 1.0, 1.0f },
		{ 1.0f, 0.0f },
		{ 0.0f, 0.0f }
	};

	const glm::vec3 facesVertex[6][4] = {
		// FRONT (Z +1)
		{
			{-1, -1,  1},
			{ 1, -1,  1},
			{ 1,  1,  1},
			{-1,  1,  1}
		},

		// BACK (Z -1)
		{
			{ 1, -1, -1},
			{-1, -1, -1},
			{-1,  1, -1},
			{ 1,  1, -1}
		},

		// LEFT (X -1)
		{
			{-1, -1, -1},
			{-1, -1,  1},
			{-1,  1,  1},
			{-1,  1, -1}
		},

		// RIGHT (X +1)
		{
			{ 1, -1,  1},
			{ 1, -1, -1},
			{ 1,  1, -1},
			{ 1,  1,  1}
		},

		// TOP (Y +1)
		{
			{-1,  1,  1},
			{ 1,  1,  1},
			{ 1,  1, -1},
			{-1,  1, -1}
		},

		// BOTTOM (Y -1)
		{
			{-1, -1, -1},
			{ 1, -1, -1},
			{ 1, -1,  1},
			{-1, -1,  1}
		}
	};

	//const glm::vec2 uvMaps[6][4] = {
	//	// FRONT (Z +1)
	//	{
	//		{-1, -1 },
	//		{ 1, -1 },
	//		{ 1,  1 },
	//		{-1,  1 }
	//	},

	//	// BACK (Z -1)
	//	{
	//		{ 1, -1 },
	//		{-1, -1 },
	//		{-1,  1 },
	//		{ 1,  1 }
	//	},

	//	// LEFT (X -1)
	//	{
	//		{-1, -1 },
	//		{-1, -1 },
	//		{-1,  1 },
	//		{-1,  1 }
	//	},

	//	// RIGHT (X +1)
	//	{
	//		{ 1, -1 },
	//		{ 1, -1 },
	//		{ 1,  1 },
	//		{ 1,  1 }
	//	},

	//	// TOP (Y +1)
	//	{
	//		{-1,  1 },
	//		{ 1,  1 },
	//		{ 1,  1 },
	//		{-1,  1 }
	//	},

	//	// BOTTOM (Y -1)
	//	{
	//		{-1, -1 },
	//		{ 1, -1 },
	//		{ 1, -1 },
	//		{-1, -1 }
	//	}
	//};

private:
	TerrainData terrainData;
};

