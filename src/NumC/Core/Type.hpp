#pragma once

#define TYPENAME(TYPE) #TYPE;

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace NumC
{
    /// Familiar names for fundamental data types.
    using float32 = float;
    using float64 = double;
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;

    /// Aliases for types common across lib.
    using size_t = int;
    using size_t_v = std::vector<size_t>;
    using shape_t = size_t_v;
    using stride_t = size_t_v;
    using indices_t = std::pair<size_t, size_t>;
    using indices_t_v = std::vector<indices_t>;
}