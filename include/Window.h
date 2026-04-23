//
// Created by inter on 4/3/2026.
//

#ifndef PROJEKTGRAFIKAKOMPUTEROWA_WINDOW_H
#define PROJEKTGRAFIKAKOMPUTEROWA_WINDOW_H

#include <GLFW/glfw3.h>
#include "Object.h"

class Window {
public:
    const float WIDTH = 2000.0f;
    const float HEIGHT = 1500.0f;

    static void framebuffer_size_callback(GLFWwindow* window, int w, int h);
    static void processInput(GLFWwindow* window, Object &object) ;
    void drawLine(int x1, int y1, int x2, int y2);
};

#endif //PROJEKTGRAFIKAKOMPUTEROWA_WINDOW_H