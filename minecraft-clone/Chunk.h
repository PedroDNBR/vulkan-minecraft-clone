#include <glm/glm.hpp>
#include <array>
#include "BlockType.h"
#include "Faces.h"

struct Chunk
{
	glm::ivec3 chunkCoord;
	std::array<BlockType, 32768> blocks;
};
