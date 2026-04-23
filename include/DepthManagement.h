//
// Created by inter on 4/12/2026.
//
#ifndef PROJEKTGRAFIKAKOMPUTEROWA_MODEL_H
#define PROJEKTGRAFIKAKOMPUTEROWA_MODEL_H
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class DepthManagement {
public:
    struct TriangleData {
        unsigned int originalIndices[3];
        glm::vec3 v[3];
        float maxZ;
        float minZ;
        float maxX;
        float minX;
        float maxY;
        float minY;
    };

    std::vector<unsigned int> sort(
        const std::vector<float>& vertices,
        const std::vector<unsigned int>& indices,
        const float* modelMatrix,
        const float* viewMatrix
    );

    bool checkIfCrossed(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);
    bool checkIfInTriangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d);
};


#endif //PROJEKTGRAFIKAKOMPUTEROWA_MODEL_H