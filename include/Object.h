//
// Created by inter on 4/2/2026.
//

#ifndef PROJEKTGRAFIKAKOMPUTEROWA_OBJECTS_H
#define PROJEKTGRAFIKAKOMPUTEROWA_OBJECTS_H
#include <string>
#include <vector>
#include <cmath>



class Object {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> colors;
    std::vector<float> textures;

    float modelMatrix[16];
    float projectionMatrix[16];
    float viewMatrix[16];

    float scale = 1.0f;

    const float translation = 0.005f;
    float translationX = 0.0f;
    float translationY = 0.0f;
    float translationZ = -10.0f;

    const double PI = std::acos(-1.0);
    const float rotation = 1.0f;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f;

    float cameraPosition[3] = {0.0f, 2.0f, 10.0f};
    float cameraU[3] = {1.0f, 0.0f, 0.0f};
    float cameraV[3] = {0.0f, 1.0f, 0.0f};
    float cameraW[3] = {0.0f, 0.0f, 1.0f};
    const float world[3] = {0.0f, 1.0f, 0.0f};
    const float angle = 0.05f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    float cameraSpeed = 0.01f;

    float fov = 45.0f;
    const float changeFov = 0.25f;


    bool loadFromFile(const std::string& path, bool ifTexture);
    void resetModelMatrix();

    void modelMatrixOperations();
    void changeScale(bool b);

    void changeRotationX(bool b);
    void changeRotationY(bool b);
    void changeRotationZ(bool b);

    void changeTranslationX(bool b);
    void changeTranslationY(bool b);
    void changeTranslationZ(bool b);

    void projectionMatrixOperations();
    void changeFOV(bool b);

    void viewMatrixOperations();
    void changeCameraVectors();
    void changeYaw(bool b);
    void changePitch(bool b);
    void changeRoll(bool b);
    void moveCamera(int i);

    float identityMatrix[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
};


#endif //PROJEKTGRAFIKAKOMPUTEROWA_OBJECTS_H