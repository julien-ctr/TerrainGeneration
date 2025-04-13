#include "UI.hpp"

UI::UI() {}

void UI::init(GLFWwindow* window) {
    // Initiate ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::render(Parameters& parameters, GLFWwindow* window, Camera& camera, unsigned int textureColorbuffer, TerrainManager& terrainGen, Stats& stats) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

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
    if (ImGui::Checkbox("Wireframe", &parameters.wireframeMode)) {
        glPolygonMode(GL_FRONT_AND_BACK, &parameters.wireframeMode ? GL_LINE : GL_FILL);
    }

    bool needRegen = false;
    if (ImGui::SliderInt("Scale", &parameters.scale, 1, 50)) needRegen = true;
    if (ImGui::SliderInt("Mesh Width", &parameters.meshW, 3, 50)) needRegen = true;
    if (ImGui::SliderInt("Mesh Height", &parameters.meshH, 3, 50)) needRegen = true;
    if (ImGui::SliderInt("Octaves", &parameters.octaves, 1, 10)) needRegen = true;
    if (ImGui::SliderFloat("Persistence", &parameters.persistence, 0.1f, 1.0f)) needRegen = true;
    if (ImGui::SliderFloat("Lacunarity", &parameters.lacunarity, 1.0f, 5.0f)) needRegen = true;

    if (parameters.autoReload && needRegen) {
        terrainGen.generate(parameters);
    }

    if (ImGui::Button("Regenerate Terrain")) {
        terrainGen.generate(parameters);
    }
    ImGui::Checkbox("Auto generate", &parameters.autoReload);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(menuWidth, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(menuWidth, menuHeight), ImGuiCond_Always);
    ImGui::Begin("Settings 2", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::ColorPicker3("Background", parameters.color);
    if (ImGui::SliderFloat("Camera Speed", &parameters.camSpeed, 1, 25)) {
        camera.SetSpeed(parameters.camSpeed);
    }
    if (ImGui::Checkbox("Voxel mode", &parameters.voxelMode)) {
        terrainGen.switchMode();
        terrainGen.generate(parameters);
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0.0f, menuHeight), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1920, windowHeight * (1.0f - visuRatio)), ImGuiCond_Always);
    ImGui::Begin("Statistics", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::Text(("Number of triangles: " + std::to_string(stats.triangleCount)).c_str());
    ImGui::Text(("FPS: " + std::to_string(stats.fps)).c_str());
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
}
