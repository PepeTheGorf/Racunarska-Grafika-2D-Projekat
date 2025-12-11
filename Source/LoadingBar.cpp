#include "../Header/LoadingBar.h"
#include <GL/glew.h>

LoadingBar::LoadingBar(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), progress(0.0f)
{
    float vertices[] = {
        -width / 2, -height / 2,
         width / 2, -height / 2,
         width / 2,  height / 2,
        -width / 2,  height / 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}


LoadingBar::~LoadingBar() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void LoadingBar::draw(unsigned int shader)
{
    glUseProgram(shader);
    glBindVertexArray(VAO);

    glUniform1i(glGetUniformLocation(shader, "uIsLoadingBar"), true);
    glUniform1f(glGetUniformLocation(shader, "uProgress"), progress);
    glUniform2f(glGetUniformLocation(shader, "uSize"), 1.0f, 1.0f);


    glUniform2f(glGetUniformLocation(shader, "uOffset"), x, y);

    glUniform1i(glGetUniformLocation(shader, "uUseTexture"), false);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glUniform1i(glGetUniformLocation(shader, "uIsLoadingBar"), false);
    glBindVertexArray(0);
}




