#ifndef FARFIELD_CHUNKPOS_H
#define FARFIELD_CHUNKPOS_H

#include <iostream>
#include <functional>
#include <ostream>
#include <glm/glm.hpp>
#include "Seed.h"

struct BlockPos
{
    int x, y, z;

    bool operator==(const BlockPos& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    static BlockPos fromWorld(const int wx, const int wy, const int wz)
    {
        return {wx & 15, wy & 15, wz & 15};
    }

    static BlockPos fromWorld(const glm::vec3& pos)
    {
        return {static_cast<int>(pos.x) & 15, static_cast<int>(pos.y) & 15, static_cast<int>(pos.z) & 15};
    }
};

struct ChunkPos {
    static constexpr unsigned char ChunkSize = 16;
    int x, y, z;

    static ChunkPos fromWorld(const int wx, const int wy, const int wz)
    {
        return {wx >> 4, wy >> 4, wz >> 4};
    }

    static ChunkPos fromWorld(const glm::vec3& pos)
    {
        return {
            static_cast<int>(pos.x) >> 4,
            static_cast<int>(pos.y) >> 4,
            static_cast<int>(pos.z) >> 4
        };
    }

    // Convert a 1D index to 3D coordinates (x, y, z)
    static BlockPos indexToLocalCoords(const int index)
    {
        return {
            index % ChunkSize,
            (index / ChunkSize) % ChunkSize,
            index / (ChunkSize * ChunkSize)
        };
    }

    // Convert a local 3D coordinates to a 1D index
    static int localCoordsToIndex(const uint8_t lx, const uint8_t ly, const uint8_t lz)
    {
        return lx + ChunkSize * (ly + ChunkSize * lz);
    }

    ChunkPos operator*(const int mul) const
    {
        return {this->x * mul, this->y * mul, this->z * mul};
    }

    bool operator<(const ChunkPos& other) const
    {
        if (x != other.x) return x < other.x;
        if (y != other.y) return y < other.y;
        return z < other.z;
    }

    bool operator==(const ChunkPos& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const ChunkPos& other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    std::ostream& operator<<(std::ostream& os) const
    {
        return os << this->x << ", " << this->y << ", " << this->z;
    }
};

inline std::string operator+(const std::string &lhs, const ChunkPos & cp)
{
    return lhs + std::to_string(cp.x) + ", " + std::to_string(cp.y) + ", " + std::to_string(cp.z);
}

struct ChunkPosHash {
    size_t operator()(const ChunkPos& p) const noexcept {
        uint64_t h = RNG::mix64(static_cast<uint64_t>(static_cast<uint32_t>(p.x)) * 0xA0761D6478BD642Full);
        h = RNG::mix64(h ^ (static_cast<uint64_t>(static_cast<uint32_t>(p.y)) * 0xE7037ED1A0B428DBull));
        h = RNG::mix64(h ^ (static_cast<uint64_t>(static_cast<uint32_t>(p.z)) * 0x8EBC6AF09C88C6E3ull));
        return h;
    }
};

#endif