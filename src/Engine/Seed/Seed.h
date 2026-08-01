#ifndef FARFIELD_SEED_H
#define FARFIELD_SEED_H

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

    uint64_t next() {
        s += 0x9E3779B97F4A7C15ull;
        return mix64(s);
    }

    double nextDouble()
    {
        return static_cast<double>(next() >> 11) * 0x1.0p-53;
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

    [[nodiscard]] int32_t range(const int32_t min, const int32_t max) {
        if (max <= min)
            return min;
        return min + static_cast<int32_t>(nextInt(static_cast<uint32_t>(max - min) + 1u));
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

//  domains :
// feature.tree.oak.count
// feature.tree.oak.position
// feature.tree.oak.height
//
// feature.tree.birch.count
// feature.tree.birch.position
//
// feature.ore.iron.count
// feature.ore.iron.position
//
// feature.lake.water.exists
// feature.lake.water.size
//
// feature.structure.village.exists
// feature.structure.village.rotation

#endif
