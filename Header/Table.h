#pragma once
#include <GL/glew.h>

class Table {
public:
    float x, y, width, height;
    unsigned int texture;
    unsigned int VAO, VBO;

    Table(float x, float y, float width, float height, unsigned int tex);
    ~Table();

    void draw(unsigned int shader);
};
