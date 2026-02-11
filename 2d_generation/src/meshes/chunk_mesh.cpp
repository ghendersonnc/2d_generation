// ReSharper disable CppClangTidyPerformanceNoIntToPtr
#include "meshes/chunk_mesh.h"

#include <iostream>
#include <utility>

#include <glm/gtc/matrix_transform.hpp>

#include <fw_graphics/vertex_array.h>
#include <fw_graphics/vertex_buffer.h>
#include <fw_graphics/index_buffer.h>
#include "config/config.h"
Fw::Graphics::Meshes::ChunkMesh::ChunkMesh(Engine::Chunk& chunk) {
    p_Chunk = &chunk;
    sGenerateMesh(_vertices, _indices, _elementCount, p_Chunk->chunkTiles);
}

Fw::Graphics::Meshes::ChunkMesh::ChunkMesh(ChunkMesh& other) : Mesh(other) {
    p_Chunk = other.p_Chunk;
    _vertexBuffer = other._vertexBuffer;
    _vertexArray = other._vertexArray;
    _indexBuffer = other._indexBuffer;

    /*
     * Prevent buffers from being deleted when copying this
     */
    other._vertexBuffer.removeId();
    other._vertexArray .removeId();
    other._indexBuffer.removeId();
}

Fw::Graphics::Meshes::ChunkMesh::~ChunkMesh() {
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
    _vertexArray.destroy();
}

void Fw::Graphics::Meshes::ChunkMesh::drawElements(Shader& shader) {
    if (_vertices.empty() || _indices.empty())
    {
        return;
    }

    if (!_canDraw)
    {
        this->handleBuffersAndArrays();
        _canDraw = true;
        return;
    }

    this->handleUniforms(shader);
    glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, nullptr);
}

void Fw::Graphics::Meshes::ChunkMesh::handleBuffersAndArrays() {
    _vertexArray.bind();
    const long long vertexBufferSize = static_cast<long long>(sizeof(Vertex)) * static_cast<GLsizeiptr>(_vertices.size());
    const long long indexBufferSize = static_cast<long long>(sizeof(int)) * static_cast<GLsizeiptr>(_indices.size());
    _vertexBuffer.setData(_vertices, vertexBufferSize);
    _indexBuffer.setData(_indices, indexBufferSize);
    const auto vertexPositionOffset = reinterpret_cast<void*>(offsetof(Vertex, vertexPosition));
    const auto colorValueOffset = reinterpret_cast<void*>(offsetof(Vertex, colorValue));
    _vertexArray.linkAttributes(_vertexBuffer, 0, 2, GL_BYTE, sizeof(Vertex), vertexPositionOffset);
    _vertexArray.linkAttributes(_vertexBuffer, 1, 3, GL_FLOAT, sizeof(Vertex), colorValueOffset);
}

void Fw::Graphics::Meshes::ChunkMesh::handleUniforms(const Shader& shader) {
    using namespace Config::Chunk;
    using namespace Config::Window;
    constexpr int zoomFactor = 6;

    constexpr glm::vec2 size(zoomFactor);
    _vertexBuffer.bind();
    _vertexArray.bind();
    auto model = glm::mat4(1.0f);

    const float positionOnScreenX = static_cast<float>(chunkSize * p_Chunk->positionInWorld.first * zoomFactor); // TODO: add camera position to this
    const float positionOnScreenY = static_cast<float>(chunkSize * p_Chunk->positionInWorld.second * zoomFactor); // TODO: add camera position to this

    model = glm::translate(model, glm::vec3(positionOnScreenX, positionOnScreenY, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.));
    const auto projection = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);

    shader.setUniformMat4F("model", model);
    shader.setUniformMat4F("projection", projection);
}

void Fw::Graphics::Meshes::ChunkMesh::sGenerateMesh(std::vector<Vertex>& vertices, std::vector<int>& indices, int& elementCount, std::vector<Engine::Tile>& chunkTiles) {
    using namespace Fw::Config::Chunk;

    constexpr int chunkVolume = chunkSize * chunkSize;
    vertices.reserve(chunkVolume);

    int currentVertex = 0;
    for (int y = 0; y < chunkSize; y++)
    {
        for (int x = 0; x < chunkSize; x++)
        {
            float color[3];
            color[0] = chunkTiles[y * chunkSize + x].color[0];
            color[1] = chunkTiles[y * chunkSize + x].color[1];
            color[2] = chunkTiles[y * chunkSize + x].color[2];
            vertices.emplace_back(x + 1, y + 1, color[0], color[1], color[2]);
            vertices.emplace_back(x + 1, y, color[0], color[1], color[2]);
            vertices.emplace_back(x, y, color[0], color[1], color[2]);
            vertices.emplace_back(x, y + 1, color[0], color[1], color[2]);

            indices.push_back(currentVertex + 0);
            indices.push_back(currentVertex + 1);
            indices.push_back(currentVertex + 2);
            indices.push_back(currentVertex + 0);
            indices.push_back(currentVertex + 2);
            indices.push_back(currentVertex + 3);
            currentVertex += 4;
        }
    }
    elementCount = static_cast<int>(indices.size());
}
