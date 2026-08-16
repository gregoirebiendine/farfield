#ifndef FARFIELD_SEED_H
#define FARFIELD_SEED_H

#include <chrono>
#include <cstdint>
#include <cstddef>
#include <string_view>

struct RNG
{
    uint64_t s;

    static uint64_t mix64(uint64_t n)
    {
        n = (n ^ (n >> 30)) * 0xBF58476D1CE4E5B9ull;
        n = (n ^ (n >> 27)) * 0x94D049BB133111EBull;
        return n ^ (n >> 31);
    }

    static uint64_t randomU64()
    {
        return mix64(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    uint64_t next() {
        s += 0x9E3779B97F4A7C15ull;
        return mix64(s);
    }

    uint32_t nextInt(const uint32_t bound) {
        if (bound == 0)
            return 0;

        auto x = static_cast<uint32_t>(this->next() >> 32);
        uint64_t m = static_cast<uint64_t>(x) * bound;
        auto low = static_cast<uint32_t>(m);

        if (low < bound) {
            const uint32_t seuil = (0u - bound) % bound;

            while (low < seuil) {
                x = static_cast<uint32_t>(this->next() >> 32);
                m = static_cast<uint64_t>(x) * bound;
                low = static_cast<uint32_t>(m);
            }
        }
        return static_cast<uint32_t>(m >> 32);
    }

    int32_t range(const int32_t min, const int32_t max) {
        if (max <= min)
            return min;
        return min + static_cast<int32_t>(nextInt(static_cast<uint32_t>(max - min) + 1u));
    }

    double nextDouble()
    {
        return static_cast<double>(next() >> 11) * 0x1.0p-53;
    }

    void shuffle(uint8_t* data, const size_t n) {
        for (size_t i = n; i > 1; --i) {
            const size_t j = this->nextInt(static_cast<uint32_t>(i));
            const uint8_t tmp = data[i-1];
            data[i-1] = data[j];
            data[j] = tmp;
        }
    }
};

struct Seed
{
    private:
        struct Raw {};
        Seed(const uint64_t raw, Raw) : v(raw) {}

    public:
        uint64_t v;

        explicit Seed(const uint64_t playerSeed) :
            v(RNG::mix64(playerSeed ^ 0x6A09E667F3BCC909ull))
        {}

        [[nodiscard]] uint64_t at(const int64_t x, const int64_t z) const {
            uint64_t h = this->v;
            h = RNG::mix64(h ^ (static_cast<uint64_t>(x) * 0xA0761D6478BD642Full));
            h = RNG::mix64(h ^ (static_cast<uint64_t>(z) * 0x8EBC6AF09C88C6E3ull));
            return h;
        }

        [[nodiscard]] Seed child(const uint64_t sel) const {
            return Seed{ RNG::mix64(this->v ^ RNG::mix64(sel + 0x9E3779B97F4A7C15ull)), Raw{} };
        }

        static constexpr uint64_t fnv1a64(const std::string_view str) {
            uint64_t h = 0xCBF29CE484222325ull;
            for (const char c : str) {
                h = h ^ static_cast<uint8_t>(c);
                h *= 0x100000001B3ull;
            }
            return h;
        }
};

class Domain
{
    uint64_t seed;

    public:
        explicit Domain(const uint64_t s) : seed(s) {}

        // derive a sub-domain from a salt
        [[nodiscard]] Domain child(const uint64_t salt) const
        {
            return Domain(RNG::mix64(seed ^ RNG::mix64(salt)));
        }

        // positional hashes, 2D and 3D
        [[nodiscard]] uint64_t hashAt(int x, int z) const;
        [[nodiscard]] uint64_t hashAt(int x, int y, int z) const;

        // RNG seeded at a position
        [[nodiscard]] RNG rngAt(const int x, const int z) const
        {
            return RNG(hashAt(x, z));
        }
};

namespace SeedFeatures
{
    namespace Terrain
    {
        constexpr auto height = Seed::fnv1a64("feature.terrain.height");
        constexpr auto tree_cell = Seed::fnv1a64("feature.terrain.tree.cell");
        constexpr auto tree_shape = Seed::fnv1a64("feature.terrain.tree.shape");
    }

    namespace Tree
    {
        constexpr auto oak = Seed::fnv1a64("feature.tree.oak");
        constexpr auto dark_oak = Seed::fnv1a64("feature.tree.dark_oak");
        constexpr auto spruce = Seed::fnv1a64("feature.tree.spruce");
        constexpr auto birch = Seed::fnv1a64("feature.tree.birch");
        constexpr auto jungle = Seed::fnv1a64("feature.tree.jungle");
        constexpr auto acacia = Seed::fnv1a64("feature.tree.acacia");
        constexpr auto azalea = Seed::fnv1a64("feature.tree.azalea");
        constexpr auto cherry = Seed::fnv1a64("feature.tree.cherry");
    }

    namespace Ore
    {
        constexpr auto coal = Seed::fnv1a64("feature.ore.coal");
        constexpr auto iron = Seed::fnv1a64("feature.ore.iron");
        constexpr auto gold = Seed::fnv1a64("feature.ore.gold");
        constexpr auto redstone = Seed::fnv1a64("feature.ore.redstone");
        constexpr auto lapis = Seed::fnv1a64("feature.ore.lapis");
        constexpr auto diamond = Seed::fnv1a64("feature.ore.diamond");
        constexpr auto netherite = Seed::fnv1a64("feature.ore.netherite");
    }
}

struct WorldSeeds
{
    Domain root;
    Domain height;
    Domain treeCells;
    Domain treeShape;

    explicit WorldSeeds(const uint64_t seed) :
        root(RNG::mix64(seed)),
        height(root.child(SeedFeatures::Terrain::height)),
        treeCells(root.child(SeedFeatures::Terrain::tree_cell)),
        treeShape(root.child(SeedFeatures::Terrain::tree_shape)) {}
};


// choice from seed :
// int in [0, N)
// int type = h % 5;

// bool % p
// bool yes = (h >> 40) < (uint64_t)(0.05 * (1ull << 24));   // 5 %

// float in [0, 1)
// double f = (h >> 11) * 0x1.0p-53;

#endif
