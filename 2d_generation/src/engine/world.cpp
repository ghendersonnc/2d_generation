#include "world.h"


#include <fw_utility/file_loaders.h>
#include "meshes/chunk_mesh.h"

Fw::Engine::World::World() {
    constexpr int chunkX = 6;
    constexpr int chunkY = 3;
    for (int x = 0; x < chunkX; x++)
    {
        for (int y = 0; y < chunkY; y++)
        {
            _chunks.try_emplace(std::tuple{ x, y }, std::pair{ x,y });
        }
    }
    constexpr auto meshCount = chunkX * chunkY;
    _meshes.reserve(meshCount);

    for (auto chunk = _chunks.begin(); chunk != _chunks.end();)
    {
        chunk->second.generateData();
        _meshes.emplace_back(chunk->second);
        _chunkCount++;
        ++chunk;
    }

}

void Fw::Engine::World::update() {
    // here would update things that exist in the world
}

void Fw::Engine::World::render(std::unordered_map<Config::Shader::Name, Graphics::Shader>& shaders) {
    for (int i = 0; i < _chunkCount; i++)
    {
        shaders[Config::Shader::CHUNK].use();
        _meshes[i].drawElements(shaders[Config::Shader::CHUNK]);
    }

}
