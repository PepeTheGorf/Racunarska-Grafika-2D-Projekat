#pragma once
#include <GL/glew.h>

class LoadingBar {
public:
    float x, y, width, height;
    float progress;
    unsigned int VAO, VBO;

    LoadingBar(float x, float y, float width, float height);
    ~LoadingBar();

    void draw(unsigned int shader);
};
