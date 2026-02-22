#include "CircleMesh2D.h"

CircleMesh2D::CircleMesh2D() {
    const float radius = 1.0f;
    const int segments = 64;
    verticies.clear();
    indicies.clear();

    verticies.push_back({ {0.0f, 0.0f}, {0.5f, 0.5f} });

    for (uint32_t i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.14159265359f * (float)i / (float)segments;

        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        float u = (x / radius + 1.0f) * 0.5f;
        float v = (y / radius + 1.0f) * 0.5f;

        verticies.push_back({ {x, y}, {u, v} });
    }
    for (uint32_t i = 1; i <= segments; ++i) {
        indicies.push_back(0);
        indicies.push_back(i);
        indicies.push_back(i + 1);
    }

    index_count = (uint32_t)indicies.size();
    Upload();
}