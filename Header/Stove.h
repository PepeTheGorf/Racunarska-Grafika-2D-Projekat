#pragma once
#include <GL/glew.h>

class Stove {
private:
    float x, y, width, height;
    unsigned int texture;
    unsigned int VAO, VBO;

public:

    Stove(float x, float y, float width, float height, unsigned int tex);
    ~Stove();

    void draw(unsigned int shader);

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

};
