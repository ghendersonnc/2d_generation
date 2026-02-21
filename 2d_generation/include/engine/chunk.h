#pragma once

#include <vector>
#include <utility>

#include "OpenSimplexNoise.hpp"
#include "engine/tile.h"

namespace Fw::Engine
{
    class Chunk
    {
    public:
        std::pair<int, int> positionInWorld;
        std::vector<Tile> chunkTiles;
        Chunk() = default;
        explicit Chunk(std::pair<int, int> chunkPosition, OpenSimplexNoise& noise);
        void generateData();
        static int totalCallsToGenerator;
    private:
        bool _ready = false;
        bool _hasMesh = false;
        OpenSimplexNoise* _noise;
        static void sGenerateTilesForChunk(std::vector<Tile>& chunkTiles, const std::pair<int, int>& chunkPosition, const Chunk& chunk);
    };
}