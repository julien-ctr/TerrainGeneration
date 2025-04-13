#pragma once

struct Parameters {
    int scale = 4;
    int meshW = 10;
    int meshH = 10;
    int octaves = 3;
    float persistence = .4f;
    float lacunarity = 2.0f;

    bool autoReload = false;
    bool wireframeMode = true;

    float color[3] = { 0.53f, 0.81f, 0.92f };

    float camSpeed = 15.0f;

    bool voxelMode = false;
};

struct Stats {
    unsigned int triangleCount = 0;
    unsigned int fps = 0;
};