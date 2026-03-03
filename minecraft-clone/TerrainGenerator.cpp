#include "TerrainGenerator.h"
#include "Vertex.h"
#include <glm/gtc/noise.hpp>

void TerrainGenerator::SetTerrainSettings(TerrainData newTerrainData)
{
	terrainData = newTerrainData;
}

/*Chunk*/void TerrainGenerator::LoadChunk(int32_t cx, int32_t cy, int32_t cz)
{
	Chunk chunk;
    chunk.chunkCoord = { cx, cy, cz };

	for (int x = 0; x < Chunk::CHUNK_SIZE; x++) {
        for (int z = 0; z < Chunk::CHUNK_SIZE; z++) {
            int height;
            int worldX = cx * Chunk::CHUNK_SIZE + x;
            int worldZ = cz * Chunk::CHUNK_SIZE + z;
            SampleHeight(worldX, worldZ, height);

            for (int y = 0; y < Chunk::CHUNK_SIZE; y++) {
                int blockIndex = index(x, y, z);
                int worldY = cy * Chunk::CHUNK_SIZE + y;

                if (worldY > height)
                    chunk.blocks[blockIndex] = BlockType::AIR;
                else if (worldY == height)
                    chunk.blocks[blockIndex] = BlockType::GRASS;
                else if (worldY >= height - terrainData.dirtDepth)
                    chunk.blocks[blockIndex] = BlockType::DIRT;
                else
                    chunk.blocks[blockIndex] = BlockType::STONE;

                CarveCaves(worldY, height, chunk, blockIndex, worldX, worldZ);
            }
        }
    }
    chunk.needUpdate = true;
	uint32_t index = loadedChunks.size();
	glm::ivec3 coord = chunk.chunkCoord;
	loadedChunks.push_back(std::move(chunk));
	loadedChunksCoordsIndex[coord] = index;
}

void TerrainGenerator::SampleHeight(int worldX, int worldZ, int& height)
{
	glm::vec2 noisePosition = glm::vec2(worldX, worldZ);
	noisePosition *= terrainData.scale;
	noisePosition += terrainData.seed;
	float total = 0;
	float ampli = 1;
	float sumAmp = 0;
	float freq = 1;

	for (size_t i = 0; i < terrainData.octaves; i++)
	{
		total += glm::simplex(noisePosition * freq) * ampli;
		sumAmp += ampli;
		ampli *= .5f;
		freq *= 2.0f;
	}

	float noise = total / sumAmp;
	//float noise = glm::simplex(noisePosition);
	float clampledNoise = noise * 0.5f + 0.5f;
	height = terrainData.baseHeight + int(pow(clampledNoise, 0.3f) * terrainData.heightRange);
}

void TerrainGenerator::CarveCaves(int worldY, int height, Chunk& chunk, int blockIndex, int worldX, int worldZ)
{
	if (worldY < height - terrainData.startingCaveDepth && chunk.blocks[blockIndex] != BlockType::AIR)
	{
		float cave = glm::simplex(glm::vec3(worldX, worldY, worldZ) * terrainData.caveScale + terrainData.caveSeedOffset);
		if (cave > terrainData.caveThreshold)
			chunk.blocks[blockIndex] = BlockType::AIR;
	}
}

bool TerrainGenerator::isNeighborBlockSolid(const glm::vec3& face, const int32_t& chunkIndex, const int32_t& x, const int32_t& y, const int32_t& z)
{
	int nx = x + face.r;
	int ny = y + face.g;
	int nz = z + face.b;

	if (nx >= 0 && nx < Chunk::CHUNK_SIZE &&
		ny >= 0 && ny < Chunk::CHUNK_SIZE &&
		nz >= 0 && nz < Chunk::CHUNK_SIZE)
		return loadedChunks[chunkIndex].blocks[index(nx,ny,nz)] != BlockType::AIR;


	glm::ivec3 neighborChunk = loadedChunks[chunkIndex].chunkCoord;
	glm::ivec3 localNeighbor = glm::ivec3(nx, ny, nz);
	if (nx < 0) 
	{
		neighborChunk.x -= 1;
		localNeighbor.x = Chunk::CHUNK_SIZE - 1;
	}
	else if (nx >= Chunk::CHUNK_SIZE)
	{
		neighborChunk.x += 1;
		localNeighbor.x = 0;
	}

	if (ny < 0)
	{
		neighborChunk.y -= 1;
		localNeighbor.y = Chunk::CHUNK_SIZE - 1;
	}
	else if (ny >= Chunk::CHUNK_SIZE)
	{
		neighborChunk.y += 1;
		localNeighbor.y = 0;
	}

	if (nz < 0)
	{
		neighborChunk.z -= 1;
		localNeighbor.z = Chunk::CHUNK_SIZE - 1;
	}
	else if (nz >= Chunk::CHUNK_SIZE)
	{
		neighborChunk.z += 1;
		localNeighbor.z = 0;
	}


	auto neighborChunkIndex = loadedChunksCoordsIndex.find(neighborChunk);
	int ncIndex = index(localNeighbor.x, localNeighbor.y, localNeighbor.z);
	if (neighborChunkIndex == loadedChunksCoordsIndex.end()) 
		return false;

	if (loadedChunks[neighborChunkIndex->second].blocks[ncIndex] == BlockType::AIR)
		return false;

	return true;
}

void TerrainGenerator::generateChunkMesh(int32_t chunkIndex)
{
	loadedChunks[chunkIndex].cpuMesh = std::make_unique<CpuMesh>();

	for (uint32_t x = 0; x < Chunk::CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < Chunk::CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < Chunk::CHUNK_SIZE; z++)
			{
				int blockIndex = index(x, y, z);
				if (loadedChunks[chunkIndex].blocks[blockIndex] == BlockType::AIR)
					continue;

				for (size_t f = 0; f < 6; f++)
				{
					if (isNeighborBlockSolid(faces[f], chunkIndex, x, y, z)) continue;

					glm::vec3 blockWorldPos =
						glm::vec3(x, y, z) +
						glm::vec3(loadedChunks[chunkIndex].chunkCoord) * float(Chunk::CHUNK_SIZE);

					uint32_t startIndex = loadedChunks[chunkIndex].cpuMesh->vertices.size();
					for (size_t v = 0; v < 4; v++)
					{
						glm::vec3 vertexFaces = blockWorldPos + facesVertex[f][v] * .5f;
						glm::vec3 colorMultiplier = { 1,1,1 }/* + faces[f]*/;
						BlockDef currentblockDef = blockDef[loadedChunks[chunkIndex].blocks[blockIndex]];
						glm::ivec2 tileOffset;

						if (loadedChunks[chunkIndex].blocks[blockIndex] == BlockType::GRASS && f == 4)
							colorMultiplier = { 0.35f, 1.0f ,0.1f };

						if (f == 4)
							tileOffset = currentblockDef.topUV;
						else if (f == 5)
							tileOffset = currentblockDef.bottomUV;
						else
							tileOffset = currentblockDef.sidesUV;

						glm::vec2 uvMap = (uvMaps[v] + glm::vec2(tileOffset)) * tileSize;

						loadedChunks[chunkIndex].cpuMesh->vertices.push_back(
							{
								vertexFaces,
								colorMultiplier,
								uvMap
							}
						);
					}
					loadedChunks[chunkIndex].cpuMesh->indices.push_back(startIndex + 0);
					loadedChunks[chunkIndex].cpuMesh->indices.push_back(startIndex + 1);
					loadedChunks[chunkIndex].cpuMesh->indices.push_back(startIndex + 2);
					loadedChunks[chunkIndex].cpuMesh->indices.push_back(startIndex + 2);
					loadedChunks[chunkIndex].cpuMesh->indices.push_back(startIndex + 3);
					loadedChunks[chunkIndex].cpuMesh->indices.push_back(startIndex + 0);
				}
			}
		}
	}
	loadedChunks[chunkIndex].needUpdate = false;
}

bool TerrainGenerator::isChunkVisible(const glm::ivec3& chunkCoord, const std::array<glm::vec4, 6>& frustumPlanes)
{
	glm::vec3 min = glm::vec3(chunkCoord) * (float)Chunk::CHUNK_SIZE;
	glm::vec3 max = min + glm::vec3(Chunk::CHUNK_SIZE);

	for (size_t i = 0; i < frustumPlanes.size(); i++)
	{
		glm::vec3 planeToCompare;
		planeToCompare.x = frustumPlanes[i].x > 0 ? max.x : min.x;
		planeToCompare.y = frustumPlanes[i].y > 0 ? max.y : min.y;
		planeToCompare.z = frustumPlanes[i].z > 0 ? max.z : min.z;

		float distance = glm::dot(glm::vec3(frustumPlanes[i]), planeToCompare) + frustumPlanes[i].w;
		if (distance < 0)
			return false;
	}

	return true;
}

//Chunk chunk;
//chunk.chunkCoord = { cx, cy, cz };
//
//for (int x = 0; x < CHUNK_SIZE; x++) {
//    for (int z = 0; z < CHUNK_SIZE; z++) {
//        for (int y = 0; y < CHUNK_SIZE; y++) {
//            uint32_t blockIndex = index(x, y, z);
//            chunk.blocks[blockIndex] = BlockType::DIRT;
//        }
//    }
//}