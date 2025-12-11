#include "../Header/Stove.h"
#include "../Header/Util.h"
#include <GL/glew.h>

Stove::Stove(float x, float y, float width, float height, unsigned int tex)
    : x(x), y(y), width(width), height(height), texture(tex)
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

Stove::~Stove() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Stove::draw(unsigned int shader) {
    glUseProgram(shader);
    glUniform2f(glGetUniformLocation(shader, "uOffset"), x, y);
    glUniform4f(glGetUniformLocation(shader, "uColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform2f(glGetUniformLocation(shader, "uSize"), 1.0f, 1.0f);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader, "uTexture0"), 0);

    int useTextureLoc = glGetUniformLocation(shader, "uUseTexture");
    glUniform1i(useTextureLoc, 1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

