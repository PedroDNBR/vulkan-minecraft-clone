#pragma once
#include <cstdint>

struct TerrainData
{
	uint16_t seed = 1;
	uint8_t baseHeight = 64;
	uint8_t dirtDepth = 8;
	uint8_t startingCaveDepth = 4;
	float caveThreshold = 0.75f;
	uint32_t heightRange = 20;
	float scale = 0.02f;
	uint8_t seedOffset = 2;
	uint8_t octaves = 4;
	float caveScale = 0.035f;
	float caveSeedOffset = 1.f;
};