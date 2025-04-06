#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <random>
#define SIZE 50

void fillArrayRandom(float myArray[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      myArray[i][j] = rand() % 255;
    }
  }
}

void printArray(float** myArray, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << myArray[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void scaleArray(float **myArray, float low, float high, int rows, int cols) {
    // Find min & max values of the current array
    float minimum = myArray[0][0];
    float maximum = myArray[0][0];
        
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            float current_value = myArray[y][x];
            if (current_value > maximum) maximum = current_value;
            if (current_value < minimum) minimum = current_value;
        }
    }        
    
    // Scale the values
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            myArray[y][x] = (((myArray[y][x]-minimum)*(high-low))/(maximum-minimum))+low;
        }
    }        
}

void randomUnitaryVector(float myVector[], unsigned int size) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<> dis(0.0, 2 * 3.1415);

  double randomAngle = dis(gen);

  myVector[0] = cos(randomAngle);
  myVector[1] = sin(randomAngle);
}

float dotProduct(float *u, float *v, unsigned int size) {
  float result = 0;
  for (int n = 0; n < size; n++) {
    result += u[n] * v[n];
  }
  return result;
}

float smoothstep(float w) {
  if (w <= 0.0) {
    return 0.0;
  }
  if (w >= 1.0) {
    return 1.0;
  }
  return 6 * pow(w, 5) - 15 * pow(w, 4) + 10 * pow(w, 3);
}

float interpolate(float a, float b, float weight) {
  return a + (b - a) * smoothstep(weight);
}