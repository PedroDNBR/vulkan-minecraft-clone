#include <glm/glm.hpp>
#include <array>
#include "BlockType.h"
#include "Faces.h"

class CpuMesh;
class GpuMesh;

struct Chunk
{
public:
	glm::ivec3 chunkCoord;
	static inline constexpr uint8_t CHUNK_SIZE = 32;
	std::array<BlockType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> blocks;

	CpuMesh* cpuMesh;
	GpuMesh* gpuMesh;

	bool needUpdate;
};
