#include "world.h"


#include <fw_utility/file_loaders.h>
#include "meshes/chunk_mesh.h"

Fw::Engine::World::World() {
    constexpr int chunkLength = 4;
    for (int x = 0; x < chunkLength; x++)
    {
        for (int y = 0; y < chunkLength; y++)
        {
            _chunks.try_emplace(std::tuple{ x, y }, std::pair{ x,y });
        }
    }
    constexpr auto meshCount = chunkLength * chunkLength;
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
    // here would update chunks
}

void Fw::Engine::World::render(Graphics::Shader& shader) {

    for (int i = 0; i < _chunkCount; i++)
    {
        _meshes[i].drawElements(shader);
    }
}
