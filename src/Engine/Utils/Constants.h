#ifndef FARFIELD_CONSTANTS_H
#define FARFIELD_CONSTANTS_H

namespace Constants
{
    namespace Chunk
    {
        inline constexpr auto SIZE = 16;
        inline constexpr auto AREA = SIZE*SIZE;
        inline constexpr auto VOLUME = SIZE*SIZE*SIZE;
        inline constexpr auto PAD_SIZE = SIZE+2;
        inline constexpr auto PAD_VOLUME = PAD_SIZE*PAD_SIZE*PAD_SIZE;
    }
}

#endif
