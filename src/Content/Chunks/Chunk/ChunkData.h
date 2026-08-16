#ifndef FARFIELD_CHUNKDATA_H
#define FARFIELD_CHUNKDATA_H

#include <vector>

#include "ChunkPos.h"
#include "Material.h"

struct ChunkData {
    bool uniform = true;
    Material id = Material::pack(0, 0); // Defaults to AIR
    std::vector<Material> blocks;

    // Get Material at (x, y, z) coordinates
    Material at(const int x, const int y, const int z) const {
        return this->uniform ? this->id : this->blocks[ChunkPos::localCoordsToIndex(x, y, z)];
    }

    // Transform uniform chunk to non-uniform
    void materialize(const Material fill) {
        if (!this->uniform)
            return;
        this->blocks.assign(Constants::Chunk::VOLUME, fill);
        this->uniform = false;
    }

    // Set a block at (x, y, z)
    void set(const int x, const int y, const int z, const Material block) {
        if (this->uniform) {
            if (block == this->id)
                return;
            this->materialize(this->id);
        }
        this->blocks[ChunkPos::localCoordsToIndex(x, y, z)] = block;
    }

    // Compact a non-uniform chunk to a uniform one, if possible
    void compact() {
        if (this->uniform)
            return;

        const Material first = this->blocks[0];
        for (Material b : this->blocks)
            if (b != first)
                return;
        this->uniform = true;
        this->id = first;
        this->blocks.clear();
        this->blocks.shrink_to_fit();
    }
};

#endif
