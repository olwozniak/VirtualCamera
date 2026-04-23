#include "../include/Shader.h"
#include <glad/glad.h>
#include "Object.h"
#include <iostream>


Shader::Shader(bool b){
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (b) {
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    } else {
        glShaderSource(fragmentShader, 1, &fragmentShaderColorSource, NULL);
    }
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use(){glUseProgram(shaderProgram);}

void Shader::setMat4(const std::string &name, const float* matrixData) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, matrixData);
}

