#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

void fillArrayRandom(float myArray[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      myArray[i][j] = rand() % 255;
    }
  }
}

void scaleMap(std::vector<std::vector<float>>& heightmap, float low, float high, int rows, int cols) {
    // Find min & max values of the current array
    float minimum = heightmap[0][0];
    float maximum = heightmap[0][0];
        
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            float current_value = heightmap[y][x];
            if (current_value > maximum) maximum = current_value;
            if (current_value < minimum) minimum = current_value;
        }
    }        
    
    // Scale the values
    for (int y = 0; y < rows; y++){
        for (int x = 0; x < cols; x++){
            heightmap[y][x] = (((heightmap[y][x]-minimum)*(high-low))/(maximum-minimum))+low;
        }
    }        
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