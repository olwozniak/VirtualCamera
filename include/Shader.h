//
// Created by inter on 4/6/2026.
//
#include <iostream>
#ifndef PROJEKTGRAFIKAKOMPUTEROWA_SHADER_H
#define PROJEKTGRAFIKAKOMPUTEROWA_SHADER_H


//
// Created by inter on 4/6/2026.
//

#include "Shader.h"

class Shader {
public:
    unsigned int shaderProgram ;

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"
        "out vec3 vertexColor;\n"
        "out vec2 TexCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "   vertexColor = aColor;\n"
        "   TexCoord = aTexCoord;\n"
        "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "in vec2 TexCoord;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D texture1;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(texture1, TexCoord);\n"
        "}\n\0";

    const char *fragmentShaderColorSource = "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(vertexColor, 1.0);\n"
        "}\n\0";

    Shader(bool b);
    void use();
    void setMat4(const std::string &name, const float* matrixData) const;
};

#endif //PROJEKTGRAFIKAKOMPUTEROWA_SHADER_H