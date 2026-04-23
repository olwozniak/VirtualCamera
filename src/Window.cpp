#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"

#include <iostream>


void Window::framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void Window::processInput(GLFWwindow* window, Object &object) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        object.resetModelMatrix();
        std::cout<<"Reset\n";
    }

    //SCALE
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        object.changeScale(true);
        std::cout<<"Zoom in\n";
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        object.changeScale(false);
        std::cout<<"Zoom out\n";
    }

    //CAMERA ROTATIONS
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        object.changePitch(true);
        std::cout<<"Pitch +\n";
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        object.changePitch(false);
        std::cout<<"Pitch -\n";
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        object.changeRoll(true);
        std::cout<<"Roll +\n";
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        object.changeRoll(false);
        std::cout<<"Roll -\n";
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        object.changeYaw(true);
        std::cout<<"Yaw +\n";
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        object.changeYaw(false);
        std::cout<<"YAw -\n";
    }
    //ruch kamerą
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        object.moveCamera(1);
        std::cout<<"Camera front\n";
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        object.moveCamera(2);
        std::cout<<"Camera back\n";
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        object.moveCamera(3);
        std::cout<<"Camera left\n";
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        object.moveCamera(4);
        std::cout<<"Camera right\n";
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        object.moveCamera(5);
        std::cout<<"Camera up\n";
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        object.moveCamera(6);
        std::cout<<"Camera down\n";
    }

    //TRANSLACJE
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        object.changeTranslationZ(true);
        std::cout<<"Translation in\n";
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        object.changeTranslationZ(false);
        std::cout<<"Translation out\n";
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        object.changeTranslationX(true);
        std::cout<<"Translation left\n";
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        object.changeTranslationX(false);
        std::cout<<"Translation rigt\n";
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        object.changeTranslationY(true);
        std::cout<<"Translation up\n";
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        object.changeTranslationY(false);
        std::cout<<"Translation down\n";
    }
    //ROTACJE
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        object.changeRotationX(true);
        std::cout<<"Rotation X front\n";
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        object.changeRotationX(false);
        std::cout<<"Rotation X back\n";
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        object.changeRotationY(true);
        std::cout<<"Rotation Y right\n";
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        object.changeRotationY(false);
        std::cout<<"Rotation Y left\n";
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        object.changeRotationZ(true);
        std::cout<<"Rotation Z clockwise\n";
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        object.changeRotationZ(false);
        std::cout<<"Rotation Z reverse-clockwise\n";
    }
    //FOV
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        object.changeFOV(true);
        std::cout<<"Fov up\n";
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        object.changeFOV(false);
        std::cout<<"Fo down\n";
    }
}

