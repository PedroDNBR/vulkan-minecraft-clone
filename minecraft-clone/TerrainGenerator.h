#include <vector>
#include "Chunk.h"

class TerrainGenerator
{
public:
	void LoadChunk(uint32_t x, uint32_t y, uint32_t z);
	std::vector<Chunk> loadedChunks;

	static constexpr uint8_t CHUNK_SIZE = 32;
	static constexpr uint8_t CHUNK_HEIGHT = 32;

	inline int index(int x, int y, int z) {
		return x + CHUNK_SIZE * (z + CHUNK_SIZE * y);
	}
private:

	
};

