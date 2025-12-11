#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Patty {
private:
    unsigned int VAO, VBO;
    float x, y;
    float radiusX, radiusY;
    float colorR, colorG, colorB, colorA;
    float velocityY, placed;
    bool isFalling;

public:
    Patty(float x, float y, float radiusX, float radiusY);
    ~Patty();

    void update(float deltaTime, GLFWwindow* window);
    void draw(unsigned int shader);
    void startOrder();
    bool isFrying(float stoveY, float stoveX, float stoveWidth);
    void setPlaced(bool placedP) {
        placed = placedP;
    }
    bool getPlaced() {
        return placed;
    }
    float getX() {
        return x;
    }
    float getY() {
        return y;
    }
    void setX(float newX) {
        x = newX;
    }
    void setY(float newY) {
        y = newY;
    }


    void setColor(float r, float g, float b, float a) {
        colorR = r;
        colorG = g;
        colorB = b;
        colorA = a;
    }
};
