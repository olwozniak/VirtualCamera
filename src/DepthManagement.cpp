//
// Created by inter on 4/12/2026.
//
#include <glm/gtc/type_ptr.hpp>
#include "../include/DepthManagement.h"
#include <algorithm>

std::vector<unsigned int> DepthManagement::sort(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,
    const float* modelMatrix, const float* viewMatrix) {

    std::vector<TriangleData> triangles;
    glm::mat4 modelMat = glm::make_mat4(modelMatrix);
    glm::mat4 viewMat = glm::make_mat4(viewMatrix);

    for (int i = 0; i < indices.size(); i+=3) {
        TriangleData tempTriangle;
        tempTriangle.originalIndices[0] = indices[i];
        tempTriangle.originalIndices[1] = indices[i+1];
        tempTriangle.originalIndices[2] = indices[i+2];

        float x = vertices[tempTriangle.originalIndices[0]*3];
        float y = vertices[tempTriangle.originalIndices[0]*3 + 1];
        float z = vertices[tempTriangle.originalIndices[0]*3 + 2];
        glm::vec4 localPos = glm::vec4(x, y, z, 1.0f);
        glm::vec4 cameraPos = viewMat * modelMat * localPos;
        tempTriangle.v[0] = glm::vec3(cameraPos);
        x = vertices[tempTriangle.originalIndices[1]*3];
        y = vertices[tempTriangle.originalIndices[1]*3 + 1];
        z = vertices[tempTriangle.originalIndices[1]*3 + 2];
        localPos = glm::vec4(x, y, z, 1.0f);
        cameraPos = viewMat * modelMat * localPos;
        tempTriangle.v[1] = glm::vec3(cameraPos);
        x = vertices[tempTriangle.originalIndices[2]*3];
        y = vertices[tempTriangle.originalIndices[2]*3 + 1];
        z = vertices[tempTriangle.originalIndices[2]*3 + 2];
        localPos = glm::vec4(x, y, z, 1.0f);
        cameraPos = viewMat * modelMat * localPos;
        tempTriangle.v[2] = glm::vec3(cameraPos);

        tempTriangle.minZ = std::min({tempTriangle.v[0].z,tempTriangle.v[1].z,tempTriangle.v[2].z});
        tempTriangle.maxZ = std::max({tempTriangle.v[0].z,tempTriangle.v[1].z,tempTriangle.v[2].z});
        tempTriangle.minX = std::min({tempTriangle.v[0].x,tempTriangle.v[1].x,tempTriangle.v[2].x});
        tempTriangle.maxX = std::max({tempTriangle.v[0].x,tempTriangle.v[1].x,tempTriangle.v[2].x});
        tempTriangle.minY = std::min({tempTriangle.v[0].y,tempTriangle.v[1].y,tempTriangle.v[2].y});
        tempTriangle.maxY = std::max({tempTriangle.v[0].y,tempTriangle.v[1].y,tempTriangle.v[2].y});

        triangles.push_back(tempTriangle);
    }

    std::sort(triangles.begin(), triangles.end(), [](const TriangleData& a, const TriangleData& b) {
        return  a.minZ< b.minZ;
    });

    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (int i = 0; i < triangles.size() - 1; i++){
            for (int j = i + 1; j < triangles.size(); j++) {
                bool isSafe = false;
                if (triangles[i].maxZ < triangles[j].minZ) {
                    break;
                }

                if (!isSafe) {
                    if (triangles[i].maxX < triangles[j].minX ||
                        triangles[i].minX > triangles[j].maxX ||
                        triangles[i].maxY < triangles[j].minY ||
                        triangles[i].minY > triangles[j].maxY) {
                        isSafe = true;
                        }
                }
                if (!isSafe) {
                    glm::vec3 v1 = triangles[j].v[1] - triangles[j].v[0];
                    glm::vec3 v2 = triangles[j].v[2] - triangles[j].v[0];
                    glm::vec3 normal = glm::cross(v1, v2);
                    if (glm::dot(normal, -triangles[j].v[0]) < 0.0f) {
                        normal = -normal;
                    }
                    bool isBehind = true;
                    for (int k = 0; k < 3; k++) {
                        glm::vec3 dir = triangles[i].v[k] - triangles[j].v[0];
                        if (glm::dot(normal, dir) > 0.0001f) {
                            isBehind = false;
                            break;
                        }
                    }
                    if (isBehind) isSafe = true;
                }

                if (!isSafe) {
                    glm::vec3 v3 = triangles[i].v[1] - triangles[i].v[0];
                    glm::vec3 v4 = triangles[i].v[2] - triangles[i].v[0];
                    glm::vec3 normal2 = glm::cross(v3, v4);
                    if (glm::dot(normal2, -triangles[i].v[0]) < 0.0f) {
                        normal2 = -normal2;
                    }
                    bool isFront = true;
                    for (int k = 0; k < 3; k++) {
                        glm::vec3 dir2 = triangles[j].v[k] - triangles[i].v[0];
                        if (glm::dot(normal2, dir2) < -0.0001f) {
                            isFront = false;
                            break;
                        }
                    }
                    if (isFront) isSafe = true;
                }

                if (!isSafe) {
                    bool isOverlapping = false;
                    glm::vec2 p[3] = { triangles[i].v[0], triangles[i].v[1], triangles[i].v[2] };
                    glm::vec2 q[3] = { triangles[j].v[0], triangles[j].v[1], triangles[j].v[2] };

                    for (int k = 0; k < 3; k++) {
                        for (int l = 0; l < 3; l++) {
                            if (checkIfCrossed(p[k], p[(k+1)%3], q[l], q[(l+1)%3])) {
                                isOverlapping = true;
                                break;
                            }
                        }
                        if (isOverlapping) break;
                    }
                    if (!isOverlapping) {
                        if (checkIfInTriangle(q[0], q[1], q[2], p[0])) isOverlapping = true;
                        if (checkIfInTriangle(p[0], p[1], p[2], q[0])) isOverlapping = true;
                    }
                    if (!isOverlapping) {
                        isSafe = true;
                    }
                }
                if (!isSafe) {
                    std::swap(triangles[i], triangles[j]);
                    swapped = true;
                    break;
                }
            }
            if (swapped) break;
        }
    }


    std::vector<unsigned int> sortedIndices;

    for (int i = 0; i < triangles.size(); i++) {
        sortedIndices.push_back(triangles[i].originalIndices[0]);
        sortedIndices.push_back(triangles[i].originalIndices[1]);
        sortedIndices.push_back(triangles[i].originalIndices[2]);
    }

    return sortedIndices;
}


bool DepthManagement::checkIfCrossed(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) {
    float d1 = glm::determinant(glm::mat2(b-a, c-a));
    float d2 = glm::determinant(glm::mat2(b-a, d-a));
    float d3 = glm::determinant(glm::mat2(d-c, a-c));
    float d4 = glm::determinant(glm::mat2(d-c, b-c));

    return ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0));
}

bool DepthManagement::checkIfInTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 p) {
    float d1 = glm::determinant(glm::mat2(v2-v1, v1-p));
    float d2 = glm::determinant(glm::mat2(v3-v2, v2-p));
    float d3 = glm::determinant(glm::mat2(v1-v3, v3-p));
    return !( d1 < 0 || d2 < 0 || d3 < 0) && (d1 > 0|| d2 > 0 || d3 > 0);
}