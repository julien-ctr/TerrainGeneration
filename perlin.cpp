#include <cmath>
#include <iostream>
#include <random>

#include "perlin.hpp"
#include "utils.hpp"

PerlinNoise::PerlinNoise(const int sizes[2], const int mesh_size[2]) {
    // Copy sizes and mesh_size
    std::copy(sizes, sizes + 2, this->sizes);
    std::copy(mesh_size, mesh_size + 2, this->mesh_size);

    dim = 2;

    // Initialize grid
    grid = new float *[this->sizes[0]];
    for (int i = 0; i < this->sizes[0]; i++) {
        grid[i] = new float[this->sizes[1]]();
    }

    // Initialize grad_grid
    int nrows = ceil(sizes[0] / mesh_size[0]);
    int ncols = ceil(sizes[1] / mesh_size[1]);
    grad_grid = new float **[nrows + 1];

    for (int i = 0; i < nrows + 1; i++) {
        grad_grid[i] = new float *[ncols + 1];
        for (int j = 0; j < ncols + 1; j++) {
            grad_grid[i][j] = new float[2];
            randomUnitaryVector(grad_grid[i][j], 2);
        }
    }

    // Calculate Perlin noise for each point in the grid
    for (int y = 0; y < this->sizes[1]; y++) {
        for (int x = 0; x < this->sizes[0]; x++) {
            grid[y][x] = pointPerlin(x, y);
        }
    }
}

PerlinNoise::~PerlinNoise() {
    // Clean up memory
    for (int i = 0; i < sizes[0]; ++i) {
        delete[] grid[i];
    }
    delete[] grid;

    for (int i = 0; i < (sizes[0] / mesh_size[0]) + 1; ++i) {
        for (int j = 0; j < (sizes[1] / mesh_size[1]) + 1; ++j) {
            delete[] grad_grid[i][j];
        }
        delete[] grad_grid[i];
    }
    delete[] grad_grid;
}

float PerlinNoise::pointPerlin(int x, int y) {
    int mesh_x0 = x / mesh_size[1];
    int mesh_x1 = mesh_x0 + 1;
    int mesh_y0 = y / mesh_size[0];
    int mesh_y1 = mesh_y0 + 1;
    
    int theoretical_mesh_x0 = mesh_x0*mesh_size[1];
    int theoretical_mesh_x1 = mesh_x1*mesh_size[1];
    int theoretical_mesh_y0 = mesh_y0*mesh_size[0];
    int theoretical_mesh_y1 = mesh_y1*mesh_size[0];
    
    float dx = static_cast<float>(x + .5 - theoretical_mesh_x0) / mesh_size[1];
    float dy = static_cast<float>(y + .5 - theoretical_mesh_y0) / mesh_size[0];
        
    float v0[] = {static_cast<float>(x - theoretical_mesh_x0) / mesh_size[1],
                  static_cast<float>(y - theoretical_mesh_y0) / mesh_size[0]};
    float n0 = dotProduct(grad_grid[mesh_y0][mesh_x0], v0, 2);

    float v1[] = {static_cast<float>(x - theoretical_mesh_x1) / mesh_size[1],
                  static_cast<float>(y - theoretical_mesh_y0) / mesh_size[0]};
    float n1 = dotProduct(grad_grid[mesh_y0][mesh_x1], v1, 2);

    float interpolation_x0 = interpolate(n0, n1, dx);

    float v2[] = {static_cast<float>(x - theoretical_mesh_x0) / mesh_size[1],
                  static_cast<float>(y - theoretical_mesh_y1) / mesh_size[0]};
    n0 = dotProduct(grad_grid[mesh_y1][mesh_x0], v2, 2);

    float v3[] = {static_cast<float>(x - theoretical_mesh_x1) / mesh_size[1],
                  static_cast<float>(y - theoretical_mesh_y1) / mesh_size[0]};
    n1 = dotProduct(grad_grid[mesh_y1][mesh_x1], v3, 2);

    float interpolation_x1 = interpolate(n0, n1, dx);

    return interpolate(interpolation_x0, interpolation_x1, dy);
}

float **PerlinNoise::getGrid() {
    return grid;
};