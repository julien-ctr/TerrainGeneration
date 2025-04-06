#pragma once

#include <cmath>
#include <iostream>
#include <random>

#define SIZE 50

void fillArrayRandom(float myArray[SIZE][SIZE]);
void printArray(float** myArray, int rows, int cols);
void scaleArray(float** myArray, float low, float high, int rows, int cols);
void randomUnitaryVector(float myVector[], unsigned int size);
float dotProduct(float* u, float* v, unsigned int size);
float smoothstep(float w);
float interpolate(float a, float b, float weight);