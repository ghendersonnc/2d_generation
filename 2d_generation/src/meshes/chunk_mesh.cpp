#include "meshes/chunk_mesh.h"

#include <utility>

#include <glm/gtc/matrix_transform.hpp>

#include <fw_graphics/vertex_array.h>
#include <fw_graphics/vertex_buffer.h>
#include <fw_graphics/index_buffer.h>
#include "config/config.h"
#include "fw_graphics/renderer2d.h"

float Fw::Meshes::ChunkMesh::zoomFactor = static_cast<float>(Config::Window::windowWidth) / static_cast<float>(8 * Config::Chunk::chunkSize);

Fw::Meshes::ChunkMesh::ChunkMesh(Fw::Engine::Chunk& chunk) {
    p_Chunk = &chunk;
    constexpr float right = static_cast<float>(Config::Window::windowWidth) / 2.f;
    constexpr float left = right - Config::Window::windowWidth;
    constexpr float top = static_cast<float>(Config::Window::windowHeight) / 2.f;
    constexpr float bottom = top - Config::Window::windowHeight;

    _projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    sGenerateMesh(*this);
}

Fw::Meshes::ChunkMesh::ChunkMesh(ChunkMesh& other) : Mesh(other) {
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

Fw::Meshes::ChunkMesh::~ChunkMesh() {
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
    _vertexArray.destroy();
}

void Fw::Meshes::ChunkMesh::drawElements(Fw::Graphics::Shader& shader, Graphics::Renderer2D& renderer) {
    if (this->canDraw(_vertices))
    {
        this->handleUniforms(shader);
        renderer.draw(_elementCount, _vertexArray, shader);
    }
}

void Fw::Meshes::ChunkMesh::handleUniforms(const Graphics::Shader& shader) {
    using namespace Config::Chunk;
    using namespace Config::Window;

    _vertexBuffer.bind();
    _vertexArray.bind();

    auto model = glm::mat4(1.0f);
    const glm::vec2 size(zoomFactor);
    model = glm::scale(model, glm::vec3(size, 1.));

    const float positionOnScreenX = static_cast<float>(chunkSize * p_Chunk->positionInWorld.first) * zoomFactor; // TODO: add camera position to this
    const float positionOnScreenY = static_cast<float>(chunkSize * p_Chunk->positionInWorld.second) * zoomFactor; // TODO: add camera position to this
    auto view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(positionOnScreenX, positionOnScreenY, 0.0f));
    

    shader.setUniformMat4F("model", model);
    shader.setUniformMat4F("view", view);
    shader.setUniformMat4F("projection", _projectionMatrix);
}

void Fw::Meshes::ChunkMesh::clearVerticesAndIndices() {
    _vertices.clear();
    _vertices.shrink_to_fit();
    _indices.clear();
    _indices.shrink_to_fit();
}

void Fw::Meshes::ChunkMesh::sGenerateMesh(ChunkMesh& mesh) {
    using namespace Fw::Config::Chunk;

    constexpr int chunkVolume = chunkSize * chunkSize;

    mesh._vertices.reserve(chunkVolume);

    int currentVertex = 0;
    for (int y = 0; y < chunkSize; y++)
    {
        for (int x = 0; x < chunkSize; x++)
        {
            float color[3];
            color[0] = mesh.p_Chunk->chunkTiles[y * chunkSize + x].color[0];
            color[1] = mesh.p_Chunk->chunkTiles[y * chunkSize + x].color[1];
            color[2] = mesh.p_Chunk->chunkTiles[y * chunkSize + x].color[2];
            mesh._vertices.emplace_back(x + 1, y + 1, color[0], color[1], color[2]);
            mesh._vertices.emplace_back(x + 1, y, color[0], color[1], color[2]);
            mesh._vertices.emplace_back(x, y, color[0], color[1], color[2]);
            mesh._vertices.emplace_back(x, y + 1, color[0], color[1], color[2]);

            mesh._indices.push_back(currentVertex + 0);
            mesh._indices.push_back(currentVertex + 1);
            mesh._indices.push_back(currentVertex + 2);
            mesh._indices.push_back(currentVertex + 0);
            mesh._indices.push_back(currentVertex + 2);
            mesh._indices.push_back(currentVertex + 3);
            currentVertex += 4;
        }
    }
    mesh._elementCount = static_cast<int>(mesh._indices.size());
}
