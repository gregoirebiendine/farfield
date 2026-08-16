#ifndef FARFIELD_MATHS_H
#define FARFIELD_MATHS_H

namespace Maths
{
    inline int floorDiv(const int a, const int b)
    {
        return (a >= 0) ? a / b : -((-a + b - 1) / b);
    }

    inline int floorMod(const int a, const int b)
    {
        const int r = a % b;
        return (r < 0) ? r + b : r;
    }

    template <typename T = float>
    T mapRange(T v, T minIn, T maxIn, T minOut, T maxOut)
    {
        return minOut + (maxOut - minOut) * ((std::clamp(v, minIn, maxIn) - minIn) / (maxIn - minIn));
    }
}

#endif
