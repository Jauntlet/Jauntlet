#include "LineRenderer.h"
#include "../Errors.h"

#include <GL/glew.h>

// Remove after done debugging
#include<iostream>
using namespace Jauntlet;

LineRenderer::LineRenderer() : _VAO(0), _VBO(0), _shaderProgram(0) {
    _MVP = glm::mat4(1.0f);
    _color = glm::vec3(255, 255, 255);
}

void LineRenderer::init() {

    // inline code shaders cause lazy
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 MVP;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 color;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0f);\n"
        "}\n\0";

    _shaderProgram = glCreateProgram();

    int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderID);

    if (vertexShaderID == 0) {
        fatalError("New vertex shader failed to be created!");
    }

    int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderID);

    if (fragmentShaderID == 0) {
        fatalError("New vertex shader failed to be created!");
    }

    glAttachShader(_shaderProgram, vertexShaderID);
    glAttachShader(_shaderProgram, fragmentShaderID);
    glLinkProgram(_shaderProgram);

    // You are supposed to check for shader linking errors here, but its midnight and we ball.

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
}

LineRenderer::~LineRenderer() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteProgram(_shaderProgram);
}

void LineRenderer::setColor(glm::vec3 color) {
    _color = color;
}
 
void LineRenderer::drawLine(glm::vec2 start, glm::vec2 end) {

    // finish setup for line rendering
    _vertices = {
        start.x, start.y, 0,
        end.x, end.y, 0
    };

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // draw the line
    glUseProgram(_shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram, "MVP"), 1, GL_FALSE, &_MVP[0][0]);
    glUniform3fv(glGetUniformLocation(_shaderProgram, "color"), 1, &_color[0]);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_LINES, 0, 2);

}