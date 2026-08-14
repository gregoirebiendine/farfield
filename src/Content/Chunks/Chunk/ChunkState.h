#ifndef FARFIELD_CHUNKSTATE_H
#define FARFIELD_CHUNKSTATE_H

enum class ChunkState
{
    UNLOADED,

    // Pass 1: Terrain
    TERRAIN_PENDING,
    TERRAIN_GENERATING,
    TERRAIN_DONE,

    // Pass 2: Meshing
    MESH_PENDING,
    MESH_GENERATING,
    MESH_DONE,

    READY,
};

inline bool hasTerrainComplete(const ChunkState state) {
    return state >= ChunkState::TERRAIN_DONE;
}

inline bool isFullyGenerated(const ChunkState state) {
    return state >= ChunkState::MESH_DONE;
}

#endif