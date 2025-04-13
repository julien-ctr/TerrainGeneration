#include "ClassicTerrain.hpp"

ClassicTerrain::ClassicTerrain() {}

void ClassicTerrain::generateTerrain(Parameters& parameters) {
    genHeightmap(100, 100, parameters);
    getVerticesAndIndices();
    loadHeightmapToBuffer();
}

void ClassicTerrain::getHeightmap(int width, int height, Parameters& parameters) {
    std::array<int, 2> size = { width, height };
    std::array<int, 2> mesh_size = { parameters.meshW, parameters.meshH };

    heightmap = octavesHeightmap(size, mesh_size, parameters.octaves, parameters.persistence, parameters.lacunarity);
    scaleMap(heightmap, 0, parameters.scale, width, height);
}

void ClassicTerrain::getVerticesAndIndices() {
    vertices.clear();
    indices.clear();

    int mapwidth = heightmap[0].size();
    int mapdepth = heightmap.size();

    for (int z = 0; z < mapdepth; ++z) {
        for (int x = 0; x < mapwidth; ++x) {
            Vertex vertex;
            vertex.position = glm::vec3(x, heightmap[z][x], z);
            vertex.texture = glm::vec2(
                static_cast<float>(x) / (mapwidth - 1),
                static_cast<float>(z) / (mapdepth - 1)
            );
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);

            vertices.push_back(vertex);
        }
    }

    for (unsigned int z = 0; z < mapdepth - 1; ++z) {
        for (unsigned int x = 0; x < mapwidth - 1; ++x) {
            unsigned int topLeft = z * mapwidth + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (z + 1) * mapwidth + x;
            unsigned int bottomRight = bottomLeft + 1;

            if ((x + z) % 2 == 0) {
                indices.insert(indices.end(), { topLeft, bottomLeft, bottomRight });
                indices.insert(indices.end(), { topLeft, topRight, bottomRight });
            }
            else {
                indices.insert(indices.end(), { topLeft, topRight, bottomLeft });
                indices.insert(indices.end(), { topRight, bottomRight, bottomLeft });
            }
        }
    }
}

void ClassicTerrain::loadHeightmapToBuffer() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

std::vector<std::vector<float>>& ClassicTerrain::getHeightmapData() {
    return heightmap;
}

int ClassicTerrain::getIndicesSize() {
    return indices.size();
}

void ClassicTerrain::render(Camera& camera) {
    shader->use();

    glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 300.0f);
    glm::mat4 model = glm::mat4(1.0f);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);

    shader->setVec3("viewPos", camera.Position);
    shader->setVec3("lightPos", glm::vec3(50, 30, 50));

    glDrawElements(GL_TRIANGLES, getIndicesSize(), GL_UNSIGNED_INT, 0);
}

void ClassicTerrain::setShader(std::shared_ptr<Shaders> shader) {
    this->shader = shader;
}
