#pragma once
#include <tuple>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

#include <fw_graphics/shader.h>

#include "config/config.h"
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
        void render(std::unordered_map<Config::Shader::Name, Graphics::Shader>& shaders, Graphics::Renderer2D& renderer);
    private:
        std::vector<Meshes::ChunkMesh> _meshes;
        std::queue<std::pair<int, int>> _chunkQueue;
        std::unordered_map<std::tuple<int, int>, Chunk> _chunks;
        int _lastChunkPositionX;
        int _lastChunkPositionY;
        int _chunkCount = 0;
        bool _loaded;

        void generateNewChunks();
        void determineChunksToRender();
    };
}
