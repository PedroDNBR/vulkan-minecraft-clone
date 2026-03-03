#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

struct ChunkHash
{
	size_t operator()(const glm::ivec3& coord) const {
		return ((std::hash<int>()(coord.x) ^
			(std::hash<int>()(coord.y) << 1)) >> 1) ^
			(std::hash<int>()(coord.z) << 1);
	}
};