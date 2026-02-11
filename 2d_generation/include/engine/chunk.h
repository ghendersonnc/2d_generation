#pragma once

#include <vector>
#include <utility>

#include "engine/tile.h"

namespace Fw::Engine
{
    class Chunk
    {
    public:
        std::pair<int, int> positionInWorld;
        std::vector<Tile> chunkTiles;
        Chunk() = default;
        explicit Chunk(std::pair<int, int> chunkPosition);
        void generateData();

    private:
        bool _ready = false;
        bool _hasMesh = false;

        static void sGenerateTilesForChunk(std::vector<Tile>& chunkTiles, const std::pair<int, int>& chunkPosition);
    };
}