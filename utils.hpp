#pragma once

#include <cmath>
#include <iostream>
#include <random>

#define SIZE 50

void fillArrayRandom(float myArray[SIZE][SIZE]);
void scaleMap(std::vector<std::vector<float>>& heightmap, float low, float high, int rows, int cols);
float smoothstep(float w);
float interpolate(float a, float b, float weight);