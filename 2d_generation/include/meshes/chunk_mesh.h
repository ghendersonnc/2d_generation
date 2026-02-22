#pragma once
#include <vector>

#include <fw_graphics/renderer2d.h>
#include <fw_graphics/shader.h>
#include <fw_graphics/vertex_int8.h>
#include <fw_graphics/mesh.h>

#include "engine/chunk.h"

namespace Fw::Meshes
{
    class ChunkMesh : Fw::Graphics::Mesh
    {
    public:
        explicit ChunkMesh(Engine::Chunk& chunk);
        ChunkMesh(ChunkMesh& other);
        ~ChunkMesh() override;
        void drawElements(Graphics::Shader& shader, Graphics::Renderer2D& renderer) override;
    private:
        std::vector<Fw::Graphics::Vertexi8> _vertices;
        Engine::Chunk* p_Chunk;

        void handleUniforms(const Graphics::Shader& shader, Graphics::Renderer2D& renderer);
        void clearVerticesAndIndices();
        static void sGenerateMesh(ChunkMesh& mesh);
    };
}
