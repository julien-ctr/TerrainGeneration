#include "VoxelTerrain.hpp"

VoxelTerrain::VoxelTerrain() : cubeGeo() {}

void VoxelTerrain::generateTerrain(Parameters& parameters) {
	genHeightmap(100, 100, parameters);

    cubeModels.clear();

    for (unsigned int z = 0; z < heightmap.size(); ++z) {
        for (unsigned int x = 0; x < heightmap[z].size(); ++x) {
            float cube_y = std::floor(heightmap[z][x]);
            for (unsigned int y = 0; y <= cube_y; ++y) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
                cubeModels.push_back(model);
            }
        }
    }
}

void VoxelTerrain::render(Camera& camera) {
    shader->use();

    glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 300.0f);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    shader->setVec3("viewPos", camera.Position);
    shader->setVec3("lightPos", glm::vec3(50, 50, 50));

    cubeGeo.drawInstanced(cubeModels);
}

void VoxelTerrain::setShader(std::shared_ptr<Shaders> shader) {
    this->shader = shader;
}
