#include <glm/glm.hpp>
#include <array>
#include "BlockType.h"
#include "Faces.h"
#include <memory>

class CpuMesh;
class GpuMesh;

struct Chunk
{
public:
	glm::ivec3 chunkCoord;
	static inline constexpr uint8_t CHUNK_SIZE = 32;
	std::array<BlockType, CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE> blocks;

	static inline constexpr int32_t INVALID_MESH_HANDLE = -1;

	std::unique_ptr<CpuMesh> cpuMesh;
	int32_t gpuMeshIndex = INVALID_MESH_HANDLE;

	bool needUpdate;
};
