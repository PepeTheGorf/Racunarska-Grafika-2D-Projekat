#include "../Header/Ingredient.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "../Header/Util.h"

float speed = 0.75f;

Ingredient::Ingredient(float startX, float startY, float width, float height, unsigned texture, bool isKetchup, bool isMustard)
    : x(startX), y(startY), width(width), height(height), texture(texture), isKetchup(isKetchup), isMustard(isMustard), placed(false)
{
    float vertices[] = {
        -width / 2, -height / 2, 0.0f, 0.0f,
         width / 2, -height / 2, 1.0f, 0.0f,
         width / 2,  height / 2, 1.0f, 1.0f,
        -width / 2,  height / 2, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}


void Ingredient::update(float deltaTime, float stackY, GLFWwindow* window) {
    if (placed) return;

    handleMovement(deltaTime, window);

    if (!isKetchup && !isMustard) {
        float yThreshold = 0.11f;
        float xMin = -0.0085f;
        float xMax = 0.0085f;

        bool onCorrectY = (y >= stackY) && (y <= stackY + yThreshold);
        bool onCorrectX = (x >= xMin && x <= xMax);

        if (onCorrectY && onCorrectX) {
            placed = true;
        }

    }

    else {
        handleAction(stackY, window);
    }
}


void Ingredient::handleMovement(float deltaTime, GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) y += speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) y -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) x -= speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) x += speed * deltaTime;
}




void Ingredient::handleAction(float stackY, GLFWwindow* window) {
    static bool spaceWasPressed = false;
    bool spaceIsPressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);


    if (spaceIsPressed && !spaceWasPressed) {

        float plateX = 0.0f;
        float plateWidth = 0.2f;

        bool hitsPlate =
            (x >= plateX - plateWidth / 2 &&
                x <= plateX + plateWidth / 2);

        unsigned spillTex;

        preprocessTexture(spillTex, isKetchup ? "Resources/ketchup-spill.png" : "Resources/mustard-spill.png");

        if (hitsPlate) {
            spills.push_back({ x, stackY + 0.1f, spillTex });
            placed = true;
        }
        else {
            float min = -0.15f;
            float max = 0.15f;
            float randOffset = ((rand() / (float)RAND_MAX) * (max - min)) + min;


            float spillY = -0.7f - 0.15f + randOffset;
            spills.push_back({ x, spillY, spillTex });
        }
    }

    spaceWasPressed = spaceIsPressed;
}




Ingredient::~Ingredient() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void Ingredient::draw(unsigned shader) {
    glUseProgram(shader);
    glBindVertexArray(VAO);

    glUniform1i(glGetUniformLocation(shader, "uUseTexture"), true);
    glUniform1i(glGetUniformLocation(shader, "uIsLoadingBar"), false);
    glUniform4f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform2f(glGetUniformLocation(shader, "uSize"), 1.0f, 1.0f);



    if (!placed || (!isKetchup && !isMustard)) {
        glUniform2f(glGetUniformLocation(shader, "uOffset"), x, y);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glBindVertexArray(0);
}


void Ingredient::drawSpills(unsigned shader) {
    if (spills.empty()) return;

    glUseProgram(shader);
    glBindVertexArray(VAO);

    glUniform1i(glGetUniformLocation(shader, "uUseTexture"), true);
    glUniform1i(glGetUniformLocation(shader, "uIsLoadingBar"), false);
    glUniform4f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform2f(glGetUniformLocation(shader, "uSize"), 0.65f, 0.55f);



    for (const auto& s : spills) {
        glUniform2f(glGetUniformLocation(shader, "uOffset"), s.x, s.y);
        glBindTexture(GL_TEXTURE_2D, s.texture);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    glBindVertexArray(0);
}
