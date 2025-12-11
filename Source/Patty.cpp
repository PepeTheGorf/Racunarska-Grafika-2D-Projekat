#include "../Header/Patty.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float moveSpeed = 1.0f;

Patty::Patty(float x, float y, float radiusX, float radiusY)
    : x(x), y(y), radiusX(radiusX), radiusY(radiusY), velocityY(0.0f), isFalling(false), placed(false)
{
    colorR = 0.65f;
    colorG = 0.3f;
    colorB = 0.28f;
    colorA = 1.0f;

    const int segments = 30;
    float vertices[(segments + 2) * 2];

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;

    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        vertices[(i + 1) * 2] = radiusX * cos(angle);
        vertices[(i + 1) * 2 + 1] = radiusY * sin(angle);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Patty::~Patty() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Patty::startOrder() {

}

void Patty::update(float deltaTime, GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) y += moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) y -= moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) x -= moveSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) x += moveSpeed * deltaTime;
}

void Patty::draw(unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    int offsetLoc = glGetUniformLocation(shaderProgram, "uOffset");
    glUniform2f(glGetUniformLocation(shaderProgram, "uSize"), 1.0f, 1.0f);
    glUniform2f(offsetLoc, x, y);

    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    glUniform4f(colorLoc, colorR, colorG, colorB, colorA);

    int useTextureLoc = glGetUniformLocation(shaderProgram, "uUseTexture");
    glUniform1i(useTextureLoc, 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
    glBindVertexArray(0);
}

bool Patty::isFrying(float stoveY, float stoveX, float stoveWidth)
{
    float pattyLeft = x - radiusX;
    float pattyRight = x + radiusX;
    float pattyBottom = y - radiusY;

    float stoveLeft = stoveX - stoveWidth / 2.0f;
    float stoveRight = stoveX + stoveWidth / 2.0f;

    bool horizontalOverlap = (pattyRight >= stoveLeft && pattyLeft <= stoveRight);
    bool verticalContact = (pattyBottom <= stoveY);

    return horizontalOverlap && verticalContact;
}
