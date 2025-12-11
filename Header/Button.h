#pragma once
#include <GL/glew.h>

class Button {
private:
    unsigned int VAO, VBO;
    float x, y, width, height;
    bool clicked;

public:
    Button(float x, float y, float width, float height);
    ~Button();
    void draw(unsigned int shaderProgram, unsigned int texture);
    bool checkClick(double mouseX, double mouseY, int windowWidth, int windowHeight);
    bool isClicked() const { return clicked; }
    void reset() { clicked = false; }
};