#pragma once
#include <tuple>
#include <vector>
#include <unordered_map>
#include <queue>
#include <utility>

#include <fw_graphics/renderer2d.h>
#include <fw_graphics/shader.h>

#include "OpenSimplexNoise.hpp"
#include "config/config.h"
#include "meshes/chunk_mesh.h"
#include "engine/chunk.h"
#include "engine/Hash.h"

namespace Fw::Engine
{
    class World
    {
    public:
        explicit World(Graphics::Renderer2D& renderer);
        void update();
        void render(std::unordered_map<Config::Shader::Name, Graphics::Shader>& shaders);
    private:
        Graphics::Renderer2D* _renderer;
        std::vector<Meshes::ChunkMesh> _meshes;
        std::queue<std::pair<int, int>> _chunkQueue;
        std::unordered_map<std::tuple<int, int>, Chunk> _chunks;
        int _lastChunkPositionX;
        int _lastChunkPositionY;
        int _chunkCount = 0;
        bool _loaded;
        OpenSimplexNoise noise;
        void generateNewChunks();
        void determineChunksToRender();
    };
}
