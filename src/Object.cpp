#include <cmath>
#include "../include/Object.h"
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>



bool Object::loadFromFile(const std::string& path, bool ifTexture) {
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path.c_str(), reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr<<reader.Error();
        }
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (size_t s = 0; s <shapes.size();s++) {
        size_t index_offset = 0;
        for (size_t f = 0;f<shapes[s].mesh.num_face_vertices.size();f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            int mat_id = shapes[s].mesh.material_ids[f];
            float r = (rand() % 256)/255.0f;
            float g = (rand() % 256)/255.0f;
            float b = (rand() % 256)/255.0f;
            if (mat_id>=0 && ifTexture) {
                r = materials[mat_id].diffuse[0];
                g = materials[mat_id].diffuse[1];
                b = materials[mat_id].diffuse[2];
            }
            for (size_t v = 0; v< fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    textures.push_back(tx);
                    textures.push_back(ty);
                } else {
                    textures.push_back(0.0f);
                    textures.push_back(0.0f);
                }
                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);
                colors.push_back(r);
                colors.push_back(g);
                colors.push_back(b);
                indices.push_back(indices.size());
            }
            index_offset += fv;

        }
    }
    return true;
}

void Object::resetModelMatrix() {
    scale=1.0f;
    translationX = 0.0f;
    translationY = 0.0f;
    translationZ = -60.0f;
    rotationX = 0.0f;
    rotationY = 0.0f;
    rotationZ = 0.0f;
    yaw = -90.0f;
    pitch = 0.0f;;
    roll = 0.0f;
    cameraPosition[0] = 0.0f;
    cameraPosition[1] = 2.0f;
    cameraPosition[2] = 10.0f;
    fov = 45.0f;

    modelMatrixOperations();
    viewMatrixOperations();
    changeCameraVectors();
    projectionMatrixOperations();
}

void Object::modelMatrixOperations() {
    for(int i = 0; i < 16; i++) modelMatrix[i] = 0.0f;

    float cosAlpha = std::cos(rotationX);
    float sinAlpha = std::sin(rotationX);
    float sinBeta = std::sin(rotationY);
    float cosBeta = std::cos(rotationY);
    float sinGamma = std::sin(rotationZ);
    float cosGamma = std::cos(rotationZ);

    modelMatrix[0] = scale*cosAlpha*cosBeta;
    modelMatrix[1] = scale*sinAlpha*cosBeta;
    modelMatrix[2] = -scale*sinBeta;
    modelMatrix[4] = scale*(cosAlpha*sinBeta*sinGamma-sinAlpha*cosGamma);
    modelMatrix[5] = scale*(sinAlpha*sinBeta*sinGamma+cosAlpha*cosGamma);
    modelMatrix[6] = scale*cosBeta*sinGamma;
    modelMatrix[8] = scale*(cosAlpha*sinBeta*cosGamma+sinAlpha*sinGamma);
    modelMatrix[9] = scale*(sinAlpha*sinBeta*sinGamma-cosAlpha*sinGamma);
    modelMatrix[10] = scale*cosBeta*cosGamma;

    modelMatrix[12] = translationX;
    modelMatrix[13] = translationY;
    modelMatrix[14] = translationZ;

    modelMatrix[15] = 1.0f;

}

void Object::changeScale(bool b) {
    if (b) {
        scale +=0.0005f;
    } else {
        scale -=0.0005f;
    }
    modelMatrixOperations();
}


void Object::changeRotationX(bool b) {
    if (b) {
        rotationX += rotation;
    } else {
        rotationX -= rotation;
    }
    modelMatrixOperations();
}

void Object::changeRotationY(bool b) {
    if (b) {
        rotationY += rotation;
    } else {
        rotationY -= rotation;
    }
    modelMatrixOperations();
}

void Object::changeRotationZ(bool b) {
    if (b) {
        rotationZ += rotation;
    } else {
        rotationZ -= rotation;
    }
    modelMatrixOperations();
}

void Object::viewMatrixOperations() {
    for(int i = 0; i < 16; i++) viewMatrix[i] = 0.0f;
    viewMatrix[0] = cameraU[0];
    viewMatrix[1] = cameraV[0];
    viewMatrix[2] = cameraW[0];
    viewMatrix[4] = cameraU[1];
    viewMatrix[5] = cameraV[1];
    viewMatrix[6] = cameraW[1];
    viewMatrix[8] = cameraU[2];
    viewMatrix[9] = cameraV[2];
    viewMatrix[10] = cameraW[2];
    viewMatrix[12] = -cameraPosition[0]*cameraU[0] - cameraPosition[1]*cameraU[1] - cameraPosition[2]*cameraU[2];
    viewMatrix[13] = -cameraPosition[0]*cameraV[0] - cameraPosition[1]*cameraV[1] - cameraPosition[2]*cameraV[2];
    viewMatrix[14] = -cameraPosition[0]*cameraW[0] - cameraPosition[1]*cameraW[1] - cameraPosition[2]*cameraW[2];
    viewMatrix[15] = 1.0f;

}
void Object::changeCameraVectors() {
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    float yawR = yaw * (PI / 180.0f);
    float pitchR = pitch * (PI / 180.0f);
    float rollR = roll * (PI / 180.0f);

    //Wektor W - obrót wokół X
    cameraW[0] = -std::cos(yawR) * std::cos(pitchR);
    cameraW[1] = -std::sin(pitchR);
    cameraW[2] = -std::sin(yawR) * std::cos(pitchR);

    float lenW = std::sqrt(cameraW[0]*cameraW[0] + cameraW[1]*cameraW[1] + cameraW[2]*cameraW[2]);
    cameraW[0] = cameraW[0] / lenW;
    cameraW[1] = cameraW[1] / lenW;
    cameraW[2] = cameraW[2] / lenW;

    //Wektor V - obrót wokół Y - camera U x world
    cameraU[0] = world[1] * cameraW[2] - world[2] * cameraW[1];
    cameraU[1] = world[2] * cameraW[0] - world[0] * cameraW[2];
    cameraU[2] = world[0] * cameraW[1] - world[1] * cameraW[0];

    float lenU = std::sqrt(cameraU[0]*cameraU[0] + cameraU[1]*cameraU[1] + cameraU[2]*cameraU[2]);
    cameraU[0] = cameraU[0] / lenU;
    cameraU[1] = cameraU[1] / lenU;
    cameraU[2] = cameraU[2] / lenU;

    //Wektor W - obrót wokół Z - camera V x camera U
    cameraV[0] = cameraW[1] * cameraU[2] - cameraW[2] * cameraU[1];
    cameraV[1] = cameraW[2] * cameraU[0] - cameraW[0] * cameraU[2];
    cameraV[2] = cameraW[0] * cameraU[1] - cameraW[1] * cameraU[0];

    float lenV = std::sqrt(cameraV[0]*cameraV[0] + cameraV[1]*cameraV[1] + cameraV[2]*cameraV[2]);
    cameraV[0] = cameraV[0] / lenV;
    cameraV[1] = cameraV[1] / lenV;
    cameraV[2] = cameraV[2] / lenV;

    if (rollR != 0.0f) {
        float cosRoll = std::cos(rollR);
        float sinRoll = std::sin(rollR);

        float tempU[3] = {cameraU[0], cameraU[1], cameraU[2]};
        float tempV[3] = {cameraV[0], cameraV[1], cameraV[2]};

        cameraU[0] = tempU[0] * cosRoll + tempV[0] * sinRoll;
        cameraU[1] = tempU[1] * cosRoll + tempV[1] * sinRoll;
        cameraU[2] = tempU[2] * cosRoll + tempV[2] * sinRoll;

        cameraV[0] = tempV[0] * cosRoll - tempU[0] * sinRoll;
        cameraV[1] = tempV[1] * cosRoll - tempU[1] * sinRoll;
        cameraV[2] = tempV[2] * cosRoll - tempU[2] * sinRoll;
    }

    viewMatrixOperations();
}

void Object::changeYaw(bool b) {
    if (b) {
        yaw += angle;
    } else {
        yaw -= angle;
    }
    changeCameraVectors();
}
void Object::changePitch(bool b) {
    if (b) {
        pitch += angle;
    } else {
        pitch -= angle;
    }
    changeCameraVectors();

}
void Object::changeRoll(bool b) {
    if (b) {
        roll += angle;
    } else {
        roll -= angle;
    }
    changeCameraVectors();
}

void Object::moveCamera(int i) {
    if (i==1) {
        cameraPosition[0] -= cameraW[0]*cameraSpeed;
        cameraPosition[1] -= cameraW[1]*cameraSpeed;
        cameraPosition[2] -= cameraW[2]*cameraSpeed;
    } else if (i==2){
        cameraPosition[0] += cameraW[0]*cameraSpeed;
        cameraPosition[1] += cameraW[1]*cameraSpeed;
        cameraPosition[2] += cameraW[2]*cameraSpeed;
    } else if (i==3) {
        cameraPosition[0] -= cameraU[0]*cameraSpeed;
        cameraPosition[1] -= cameraU[1]*cameraSpeed;
        cameraPosition[2] -= cameraU[2]*cameraSpeed;
    } else if (i==4) {
        cameraPosition[0] += cameraU[0]*cameraSpeed;
        cameraPosition[1] += cameraU[1]*cameraSpeed;
        cameraPosition[2] += cameraU[2]*cameraSpeed;
    } else if (i==5) {
        cameraPosition[0] += cameraV[0]*cameraSpeed;
        cameraPosition[1] += cameraV[1]*cameraSpeed;
        cameraPosition[2] += cameraV[2]*cameraSpeed;
    } else if (i==6) {
        cameraPosition[0] -= cameraV[0]*cameraSpeed;
        cameraPosition[1] -= cameraV[1]*cameraSpeed;
        cameraPosition[2] -= cameraV[2]*cameraSpeed;
    }
    viewMatrixOperations();
}

void Object::changeTranslationX(bool b) {
    if (b) {
        translationX += translation;
    } else {
        translationX -=translation;
    }
    modelMatrixOperations();
}

void Object::changeTranslationY(bool b) {
    if (b) {
        translationY += translation;
    } else {
        translationY -=translation;
    }
    modelMatrixOperations();
}

void Object::changeTranslationZ(bool b) {
    if (b) {
        translationZ += translation;
    } else {
        translationZ -=translation;
    }
    modelMatrixOperations();
}


void Object::projectionMatrixOperations() {
    float d = 1.0f;
    float f = 100.0f;
    float aspect = 2000.0f / 1500.0f;
    float fovRadians = fov * (PI / 180.0f);
    float tanFov = std::tan(fovRadians / 2.0f);

    for(int i = 0; i < 16; i++) projectionMatrix[i] = 0.0f;
    projectionMatrix[0] = 1.0f / (aspect * tanFov);
    projectionMatrix[5] = 1.0f / tanFov;
    projectionMatrix[10] = -(f + d) / (f - d);
    projectionMatrix[11] = -1.0f;
    projectionMatrix[14] = -(2.0f * f * d) / (f - d);
    projectionMatrix[15] = 0.0f;

}

void Object::changeFOV(bool b) {
    if (b) {
        fov += changeFov;
    } else {
        fov -= changeFov;
    }

    if (fov < 10.0f) fov = 10.0f;
    if (fov > 120.0f) fov = 120.0f;
    projectionMatrixOperations();
}


