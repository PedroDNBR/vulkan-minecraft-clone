#include <vector>
#include "Chunk.h"
#include "BlockDef.h"

class TerrainGenerator
{
public:
	void LoadChunk(uint32_t x, uint32_t y, uint32_t z);
	std::vector<Chunk> loadedChunks;
	bool isBlockSolid(const Chunk& chunk, int x, int y, int z);

	static constexpr uint8_t CHUNK_SIZE = 16;

	inline int index(int x, int y, int z) {
		return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
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

	
};

