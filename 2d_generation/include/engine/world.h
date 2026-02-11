#pragma once
#include <tuple>
#include <unordered_map>


#include <fw_graphics/shader.h>
#include "meshes/chunk_mesh.h"
#include "engine/chunk.h"
#include "engine/Hash.h"
namespace Fw::Engine
{
    class World
    {
    public:
        World();
        void update();
        void render(Graphics::Shader& shader);
    private:
        std::vector<Graphics::Meshes::ChunkMesh> _meshes;
        std::unordered_map<std::tuple<int, int>, Engine::Chunk> _chunks;
        int _chunkCount = 0;
    };
}