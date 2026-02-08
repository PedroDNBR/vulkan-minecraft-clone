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