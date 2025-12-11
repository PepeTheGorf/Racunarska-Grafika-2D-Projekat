#include "../Header/Button.h"

Button::Button(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height), clicked(false) {

    float vertices[] = {
        -0.2f, -0.1f, 0.0f, 0.0f,
         0.2f, -0.1f, 1.0f, 0.0f,
         0.2f,  0.1f, 1.0f, 1.0f,
        -0.2f,  0.1f, 0.0f, 1.0f
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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Button::~Button() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Button::draw(unsigned int shaderProgram, unsigned int texture) {
    glUseProgram(shaderProgram);
    glUniform2f(glGetUniformLocation(shaderProgram, "uOffset"), x, y);
    glUniform4f(glGetUniformLocation(shaderProgram, "uColor"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform2f(glGetUniformLocation(shaderProgram, "uSize"), 1.0f, 1.0f);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture0"), 0);

    int useTextureLoc = glGetUniformLocation(shaderProgram, "uUseTexture");
    glUniform1i(useTextureLoc, 1); 

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}




bool Button::checkClick(double mouseX, double mouseY, int windowWidth, int windowHeight) {
    float ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / windowHeight;

    if (ndcX >= x - width / 2 && ndcX <= x + width / 2 &&
        ndcY >= y - height / 2 && ndcY <= y + height / 2) {
        clicked = true;
        return true;
    }
    return false;
}