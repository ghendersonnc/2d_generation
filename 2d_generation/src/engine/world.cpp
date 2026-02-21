#include "world.h"

#include <imgui.h>

#include <fw_utility/file_loaders.h>
#include <fw_graphics/renderer2d.h>
#include "meshes/chunk_mesh.h"

Fw::Engine::World::World(Graphics::Renderer2D& renderer) {
    _lastChunkPositionX = INT_MIN;
    _lastChunkPositionY = INT_MIN;
    _loaded = false;
    _renderer = &renderer;
    noise = OpenSimplexNoise(12345);
}

void Fw::Engine::World::update() {
    this->generateNewChunks();
    
    // TODO: 0s will come from Camera
    if (0 != _lastChunkPositionX || 0 != _lastChunkPositionY || !_chunkQueue.empty())
    {
        _lastChunkPositionX = 0;
        _lastChunkPositionY = 0;
        this->determineChunksToRender();
    }

}

void Fw::Engine::World::render(std::unordered_map<Config::Shader::Name, Graphics::Shader>& shaders) {
    ImGui::Begin("Diagnostic");
    ImGui::TextColored(ImVec4(1, 1, 0, 1),"TOTAL CALLS TO TILE GENERATOR: %d", Chunk::totalCallsToGenerator);
    ImGui::Text("FPS (a): %f", ImGui::GetIO().Framerate);
    if (ImGui::Button("Toggle GL_LINE"))
    {
        _renderer->wireframeToggle();
    }
    ImGui::End();
    for (int i = 0; i < _chunkCount; i++)
    {
        shaders[Config::Shader::CHUNK].use();
        _meshes[i].drawElements(shaders[Config::Shader::CHUNK], *_renderer);
    }

}

void Fw::Engine::World::generateNewChunks() {
    constexpr int chunkX = 5;
    constexpr int chunkY = 5;
    if (!_loaded)
    {
        for (int y = -4; y < chunkX; y++)
        {
            for (int x = -4; x < chunkY; x++)
            {
                _chunkQueue.emplace(x, y);
            }
        }
        _loaded = true;
    }

    while (!_chunkQueue.empty())
    {
        auto position = _chunkQueue.front();
        _chunkQueue.pop();
        _chunks.try_emplace(std::tuple{ position.first, position.second }, position, noise);
    }
}

void Fw::Engine::World::determineChunksToRender() {
   const auto meshCount = _chunks.size();
   _meshes.reserve(meshCount);

    for (auto chunk = _chunks.begin(); chunk != _chunks.end();)
    {
        _meshes.emplace_back(chunk->second);
        _chunkCount++;
        ++chunk;
    }
}

