#pragma once
#include <vector>

#include <fw_graphics/shader.h>
#include <fw_graphics/vertex.h>
#include <fw_graphics/mesh.h>

#include "engine/chunk.h"
#include "tile.h"

namespace Fw::Graphics::Meshes
{
    class ChunkMesh : Mesh
    {
    public:
        explicit ChunkMesh(Engine::Chunk& chunk);
        ChunkMesh(ChunkMesh& other);
        ~ChunkMesh() override;
        void drawElements(Shader& shader) override;
    private:
        Engine::Chunk* p_Chunk;

        void handleBuffersAndArrays();
        void handleUniforms(const Shader& shader);
        static void sGenerateMesh(std::vector<Vertex>& vertices, std::vector<int>& indices, int& elementCount, std::vector<Engine::Tile>& chunkTiles);
    };
}
