#pragma once
#include "utils.hpp"

class PerlinNoise {
private:
    int sizes[2];
    int dim;
    int mesh_size[2];
    float** grid;
    float*** grad_grid;

public:
    PerlinNoise(const int sizes[2], const int mesh_size[2]);
    ~PerlinNoise();
    float pointPerlin(int x, int y);
    float** getGrid();
};
