#ifndef FARFIELD_CHUNKDATA_H
#define FARFIELD_CHUNKDATA_H

#include <array>
#include "Material.h"

struct ChunkData {
    bool uniform = true;
    BlockId id = 0; // Defaults to AIR
    std::array<Material, 16*16*16> blocks{};
};

#endif
