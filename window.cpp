#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "Shaders.hpp"
#include "textureLoad.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window
int height = 800;
int width = 1200;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float pitch = 0.0f;
float yaw = -90.0f;
float roll = 0.0f;

float lastX = static_cast<float>(width) / 2;
float lastY = static_cast<float>(height) / 2;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xfloat = static_cast<float>(xpos);
    float yfloat = static_cast<float>(ypos);

    float xoffset = xfloat - lastX;
    float yoffset = lastY - yfloat;

    lastX = xfloat;
    lastY = yfloat;

    const float sensitivity = 0.1f;

    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
};


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    const float cameraSpeed = 4.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
}

int main() {
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1200, 800, "Title", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Cursor management
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build the shader program
    Shaders myShaders("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");

    glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------------------------------------------- //

    std::vector<std::vector<float>> heightmap = {
    {1.0f, 1.1f, 1.2f, 1.0f, 0.9f, 0.8f, 0.7f, 0.8f, 0.9f, 1.0f},
    {1.1f, 1.3f, 1.4f, 1.2f, 1.0f, 0.9f, 0.8f, 0.7f, 0.8f, 1.0f},
    {1.2f, 1.4f, 1.5f, 1.3f, 1.1f, 1.0f, 0.9f, 0.8f, 0.9f, 1.1f},
    {1.0f, 1.2f, 1.3f, 1.1f, 0.9f, 0.8f, 0.7f, 0.8f, 0.9f, 1.0f},
    {0.9f, 1.0f, 1.1f, 0.9f, 0.8f, 0.7f, 0.6f, 0.7f, 0.8f, 0.9f},
    {0.8f, 0.9f, 1.0f, 0.8f, 0.7f, 0.6f, 0.5f, 0.6f, 0.7f, 0.8f},
    {0.7f, 0.8f, 0.9f, 0.7f, 0.6f, 0.5f, 0.4f, 0.5f, 0.6f, 0.7f},
    {0.8f, 0.9f, 1.0f, 0.8f, 0.7f, 0.6f, 0.5f, 0.6f, 0.7f, 0.8f},
    {0.9f, 1.0f, 1.1f, 0.9f, 0.8f, 0.7f, 0.6f, 0.7f, 0.8f, 0.9f},
    {1.0f, 1.1f, 1.2f, 1.0f, 0.9f, 0.8f, 0.7f, 0.8f, 0.9f, 1.0f}
    };

    int mapwidth = heightmap[0].size();
    int mapdepth = heightmap.size();

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    for (int z = 0; z < mapdepth; ++z) {
        for (int x = 0; x < mapwidth; ++x) {
            float posX = static_cast<float>(x);
            float posY = heightmap[z][x];
            float posZ = static_cast<float>(z);

            float texX = static_cast<float>(x) / (mapwidth - 1);
            float texY = static_cast<float>(z) / (mapdepth - 1);

            vertices.push_back(posX);
            vertices.push_back(posY);
            vertices.push_back(posZ);
            vertices.push_back(texX);
            vertices.push_back(texY);
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

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture = loadTexture("Textures/dirt_texture.jpg");

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.53f, 0.81f, 0.92f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glBindTexture(GL_TEXTURE_2D, texture);

        myShaders.use();
        float time = (float)glfwGetTime();

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 1200.0f / 800.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);

        unsigned int viewLoc = glGetUniformLocation(myShaders.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(myShaders.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int modelLoc = glGetUniformLocation(myShaders.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}