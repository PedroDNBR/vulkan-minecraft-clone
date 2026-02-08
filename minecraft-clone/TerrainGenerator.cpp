#include "TerrainGenerator.h"
#include "Vertex.h"

void TerrainGenerator::LoadChunk(uint32_t cx, uint32_t cy, uint32_t cz)
{
    Chunk chunk;
    chunk.chunkCoord = { cx, cy, cz };

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                if(y == CHUNK_SIZE - 1)
                    chunk.blocks[index(x, y, z)] = BlockType::GRASS;
                else
                    chunk.blocks[index(x,y,z)] = BlockType::DIRT;
            }
        }
    }
    chunk.blocks[4] = BlockType::STONE;
    chunk.blocks[6] = BlockType::AIR;
    loadedChunks.push_back(std::move(chunk));
}

bool TerrainGenerator::isBlockSolid(const Chunk& chunk, int x, int y, int z)
{
    if(
        x < 0 || y < 0 || z < 0 ||
        x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE
        )
        return false;

    return chunk.blocks[index(x, y, z)] != BlockType::AIR;
}

void TerrainGenerator::generateChunkMesh(int32_t chunkIndex, std::vector<Vertex>& vertexData, std::vector<uint32_t>& indicesData)
{
	for (uint32_t x = 0; x < CHUNK_SIZE; x++)
	{
		for (uint32_t y = 0; y < CHUNK_SIZE; y++)
		{
			for (uint32_t z = 0; z < CHUNK_SIZE; z++)
			{
				int blockIndex = index(x, y, z);
				if (loadedChunks[chunkIndex].blocks[blockIndex] == BlockType::AIR)
					continue;

				for (size_t f = 0; f < 6; f++)
				{
					int nx = x + faces[f].r;
					int ny = y + faces[f].g;
					int nz = z + faces[f].b;

					int nIndex = index(nx, ny, nz);

					if (nx < 0 || nx >= CHUNK_SIZE ||
						ny < 0 || ny >= CHUNK_SIZE ||
						nz < 0 || nz >= CHUNK_SIZE ||
						loadedChunks[chunkIndex].blocks[nIndex] == BlockType::AIR)
					{
						glm::vec3 blockWorldPos =
							glm::vec3(x, y, z) +
							glm::vec3(loadedChunks[chunkIndex].chunkCoord) * float(CHUNK_SIZE);

						uint32_t startIndex = vertexData.size();
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

							vertexData.push_back(
								{
									vertexFaces,
									colorMultiplier,
									uvMap
								}
							);
						}
						indicesData.push_back(startIndex + 0);
						indicesData.push_back(startIndex + 1);
						indicesData.push_back(startIndex + 2);
						indicesData.push_back(startIndex + 2);
						indicesData.push_back(startIndex + 3);
						indicesData.push_back(startIndex + 0);
					}
				}
			}
		}
	}
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