#pragma once
#include <utility>
#include <cstdint>

namespace Fw::Engine
{
    struct Tile
    {
        std::pair<int, int> positionInWorld;
        std::pair<uint8_t, uint8_t> positionInChunk;
        float color[3];
    };
}
