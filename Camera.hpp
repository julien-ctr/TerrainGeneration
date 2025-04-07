#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 Position, Front, Up;
    float Pitch, Yaw, Speed, Sensitivity;

    Camera();
    Camera(float speed);
    void ProcessKeyboard(int key, float deltaTime);
    void UpdateDirection();
    void SetSpeed(float speed);
};