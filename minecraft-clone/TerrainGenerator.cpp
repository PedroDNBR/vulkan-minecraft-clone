#include "TerrainGenerator.h"

void TerrainGenerator::LoadChunk(uint32_t cx, uint32_t cy, uint32_t cz)
{
    Chunk chunk;
    chunk.chunkCoord = { cx, cy, cz };

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                uint32_t blockIndex = index(x, y, z);
                if (y < CHUNK_SIZE / 2)
                    chunk.blocks[blockIndex] = BlockType::DIRT;
                else
                    chunk.blocks[blockIndex] = BlockType::AIR;
            }
        }
    }
    loadedChunks.push_back(std::move(chunk));
}