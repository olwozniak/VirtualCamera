#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Object.h"
#include "Shader.h"
#include "DepthManagement.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int WIDTH = 2000;
const unsigned int HEIGHT = 1500;

int main(int argc, char**argv) {
    bool ifTexture = false;

    if (argc == 3) {
        ifTexture = true;
    } else if (argc == 2) {
        ifTexture = false;
    } else {
        std::cerr << "Uzycie z tekstura: " << argv[0] << " model.obj tekstura.jpg" << std::endl;
        std::cerr << "Uzycie bez tekstury: " << argv[0] << " model.obj" << std::endl;
        return -1;
    }

    Object object;
    object.loadFromFile(argv[1], ifTexture);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Kamera Wirtualna", NULL, NULL);
    if (window==NULL) {
        std::cout<<"Głupie okno"<<std::endl;
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Window::framebuffer_size_callback);

    if ( !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout <<"Błąd GLAD"<< std::endl;
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);

    Shader shader(ifTexture);

    unsigned int VBO, VAO, EBO, colorVBO, texturesVBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &colorVBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(float), &object.vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.indices.size() * sizeof(unsigned int), &object.indices[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, object.colors.size() * sizeof(float), &object.colors[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (ifTexture) {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(argv[2], &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Błąd tekstury." << std::endl;
        }
        stbi_image_free(data);
    }
    glGenBuffers(1, &texturesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texturesVBO);
    glBufferData(GL_ARRAY_BUFFER, object.textures.size() * sizeof(float), &object.textures[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    DepthManagement depthManagement;
    object.modelMatrixOperations();
    object.projectionMatrixOperations();
    object.viewMatrixOperations();
    while (!glfwWindowShouldClose(window)) {
        Window::processInput(window, object);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.setMat4("model", object.modelMatrix);
        shader.setMat4("view", object.viewMatrix);
        shader.setMat4("projection", object.projectionMatrix);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAO);
        if (!ifTexture) {
            std::vector<unsigned int> sortedIndices = depthManagement.sort( object.vertices, object.indices,
                object.modelMatrix, object.viewMatrix, object.projectionMatrix);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sortedIndices.size() * sizeof(unsigned int), &sortedIndices[0]);
            glDrawElements(GL_TRIANGLES, sortedIndices.size(), GL_UNSIGNED_INT, 0);
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, object.indices.size() * sizeof(unsigned int), &object.indices[0]);
            glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, 0);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;

}