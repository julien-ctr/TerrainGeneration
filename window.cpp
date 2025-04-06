#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include "Shaders.hpp"
#include "textureLoad.hpp"
#include "perlin.hpp"
#include "utils.hpp"
#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


// Camera
Camera camera;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int scale = 4;

bool wireframeMode = true;

unsigned int framebuffer, textureColorbuffer, rbo;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    int keys[] = {
        GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D,
        GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT,
        GLFW_KEY_I, GLFW_KEY_K, GLFW_KEY_J, GLFW_KEY_L
    };

    for (int key : keys) {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            camera.ProcessKeyboard(key, deltaTime);
    }
}

GLFWwindow* initWindow() {
    glfwInit();

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Title", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMaximizeWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void setupFramebuffer() {
    int textureWidth, textureHeight;
    textureWidth = 1920;
    textureHeight = 1080;

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, textureWidth, textureHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<std::vector<float>> getHeightmap(int width, int height, int mesh_width, int mesh_height, int scale) {
    const int sizes[2] = { width, height };
    const int mesh_size[2] = { mesh_width, mesh_height };

    PerlinNoise perlin_noise(sizes, mesh_size);
    float** rawHeightMap = perlin_noise.getGrid();
    scaleArray(rawHeightMap, 0, scale, width, height);

    std::vector<std::vector<float>> heightmap(height, std::vector<float>(width));

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            heightmap[i][j] = rawHeightMap[i][j];
        }
    }

    return heightmap;
}

void loadHeightmapToBuffer(std::vector<float>& vertices, std::vector<uint32_t>& indices, unsigned int VBO, unsigned int EBO) {
   

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void getVerticesAndIndices(std::vector<std::vector<float>>& heightmap, std::vector<float>& vertices, std::vector<uint32_t>& indices) {
    int mapwidth = heightmap[0].size();
    int mapdepth = heightmap.size();
    std::cout << mapwidth << " / " << mapdepth << std::endl;

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
}



int main() {
    GLFWwindow* window = initWindow();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initiate ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Build the shader program
    Shaders myShaders("Shaders/vertexShader.vs", "Shaders/fragmentShader.fs");

    glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------------------------------------------- //

    std::vector<std::vector<float>> heightmap = getHeightmap(100, 100, 10, 10, scale);
    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    getVerticesAndIndices(heightmap, vertices, indices);


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    loadHeightmapToBuffer(vertices, indices, VBO, EBO);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    setupFramebuffer();

    float color[3] = { 0.53f, 0.81f, 0.92f };

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera);

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glViewport(0, 0, 1920, 1080);
        glClearColor(color[0], color[1], color[2], 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShaders.use();
        float time = (float)glfwGetTime();

        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        unsigned int viewLoc = glGetUniformLocation(myShaders.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        unsigned int projectionLoc = glGetUniformLocation(myShaders.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int modelLoc = glGetUniformLocation(myShaders.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ImGui UI
        float visuRatio = .7f;
        int windowHeight, windowWidth;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);


        float menuWidth = (1.0f - visuRatio) / 2.0f * 1920;
        float menuHeight = 1080 * visuRatio;
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);
        ImGui::Begin("Settings 1", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        if (ImGui::Checkbox("Wireframe", &wireframeMode)) {
            glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
        }
        ImGui::SliderInt("Scale", &scale, 1, 10);
        if (ImGui::Button("Regenerate Terrain")) {
            heightmap = getHeightmap(100, 100, 10, 10, scale);

            vertices.clear();
            indices.clear();
            getVerticesAndIndices(heightmap, vertices, indices);

            glBindVertexArray(VAO);

            loadHeightmapToBuffer(vertices, indices, VBO, EBO);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(menuWidth, 0.0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);
        ImGui::Begin("Settings 2", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::ColorPicker3("Background", color);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(0.0f, menuHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(1920, windowHeight * (1.0f - visuRatio)), ImGuiCond_Always);
        ImGui::Begin("Settings 3", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::End();

        ImVec2 windowPos(1920 * (1.0f - visuRatio), 0);
        ImVec2 windowSize(1920 * visuRatio, 1080 * visuRatio);

        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
        ImGui::Begin("Rendered map", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImVec2 availableSize = ImGui::GetContentRegionAvail();
        ImGui::Image((ImTextureID)(intptr_t)textureColorbuffer, availableSize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}