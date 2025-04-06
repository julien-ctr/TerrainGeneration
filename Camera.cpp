#include "Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

Camera::Camera() :
    Position(glm::vec3(0.0f, 0.0f, 3.0f)),
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    Up(glm::vec3(0.0f, 1.0f, 0.0f)),
    Pitch(0.0f),
    Yaw(-90.0f),
    Speed(4.0f),
    Sensitivity(0.1f) {
}

void Camera::ProcessKeyboard(int key, float deltaTime) {
    float vel = Speed * deltaTime;
    float rotSpeed = 60.0f * deltaTime;

    switch (key) {
    case GLFW_KEY_W:  // FORWARD
        Position += Front * vel;
        break;
    case GLFW_KEY_S:  // BACKWARD
        Position -= Front * vel;
        break;
    case GLFW_KEY_A:  // LEFT
        Position -= glm::normalize(glm::cross(Front, Up)) * vel;
        break;
    case GLFW_KEY_D:  // RIGHT
        Position += glm::normalize(glm::cross(Front, Up)) * vel;
        break;
    case GLFW_KEY_SPACE:  // UP
        Position += Up * vel;
        break;
    case GLFW_KEY_LEFT_SHIFT:  // DOWN
        Position -= Up * vel;
        break;
    case GLFW_KEY_I:  // PITCH UP
        Pitch += rotSpeed;
        break;
    case GLFW_KEY_K:  // PITCH DOWN
        Pitch -= rotSpeed;
        break;
    case GLFW_KEY_J:  // YAW LEFT
        Yaw -= rotSpeed;
        break;
    case GLFW_KEY_L:  // YAW RIGHT
        Yaw += rotSpeed;
        break;
    default:
        break;
    }

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    UpdateDirection();
}

void Camera::UpdateDirection() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.y = sin(glm::radians(Pitch));
    direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(direction);
}
