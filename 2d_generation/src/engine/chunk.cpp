#include "engine/chunk.h"

#include <iostream>

#include "OpenSimplexNoise.hpp"
#include "config/config.h"

int Fw::Engine::Chunk::totalCallsToGenerator = 0;

Fw::Engine::Chunk::Chunk(const std::pair<int, int> chunkPosition, OpenSimplexNoise& noise) {
    positionInWorld = chunkPosition;
    _noise = &noise;
    generateData();
}

void Fw::Engine::Chunk::generateData() {
    totalCallsToGenerator++;
    sGenerateTilesForChunk(chunkTiles, positionInWorld, *this);
}

void Fw::Engine::Chunk::sGenerateTilesForChunk(std::vector<Tile>& chunkTiles, const std::pair<int, int>& chunkPosition, const Chunk& chunk) {
    using namespace Config::Chunk;

    constexpr int chunkVolume = chunkSize * chunkSize;
    chunkTiles.reserve(chunkVolume);

    const int noiseEvalX = chunkSize * static_cast<int>(chunkPosition.first);
    const int noiseEvalY = chunkSize * static_cast<int>(chunkPosition.second);

    for (int y = 0; y < chunkSize; y++)
    {
        for (int x = 0; x < chunkSize; x++)
        {
            Tile tile;
            tile.positionInChunk = { x, y };
            const int worldPositionX = chunkPosition.first * chunkSize + x;
            const int worldPositionY = chunkPosition.second * chunkSize + y;
            tile.positionInWorld = { worldPositionX, worldPositionY };

            if (chunkPosition.first == 0 && chunkPosition.second == 0)
            {
                if (tile.positionInWorld.first > 12 && tile.positionInWorld.first < 19 && tile.positionInWorld.second > 12 && tile.positionInWorld.second < 19)
                {
                    tile.color[0] = 0.;
                    tile.color[1] = 0.;
                    tile.color[2] = 1.;
                } else
                {
                    tile.color[0] = 150.f / 255.f;
                    tile.color[1] = 150.f / 255.f;
                    tile.color[2] = 150.f / 255.f;
                }

                

            } else
            {
                
                const float frequency = .1f;
                float amplitude = 16.0f;
                float evalX = (x + noiseEvalX) * frequency;
                float evalY = (y + noiseEvalY) * frequency;
                int noiseFinal = chunk._noise->Evaluate(evalX, evalY) * amplitude;

                auto color = static_cast<float>(noiseFinal) / chunkSize;

                if (color > .5)
                {
                    tile.color[0] = 4.f / 255.f;
                    tile.color[1] = 0.;
                    tile.color[2] = 115.f / 255.f;
                }
                else if (color >= .2 && color < .5)
                {
                    tile.color[0] = 0.;
                    tile.color[1] = 0.;
                    tile.color[2] = 1.;
                }
                else if (color < .2 && color >= .15)
                {
                    tile.color[0] = 237.f / 255.f;
                    tile.color[1] = 217.f / 255.f;
                    tile.color[2] = 114.f / 255.f;
                }
                else
                {
                    tile.color[0] = 50.f / 255.f;
                    tile.color[1] = 158.f / 255.f;
                    tile.color[2] = 0.;
                }
            }
            
            chunkTiles.emplace_back(std::move(tile));
        }
    }
}
