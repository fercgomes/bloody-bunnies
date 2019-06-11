#pragma once
#include "Components.h"
#include "../include/tiny_obj_loader.h"
#include "../GLManager.h"
#include <map>
#include <vector>
#include <glm/mat4x4.hpp>

#define PLANAR_PROJECTION 1
#define SPHERICAL_PROJECTION 0
#define TEXCOORD_PROJECTION 2

class ModelComponent : public Component
{
private:
    GLManager* glManager;
    std::string shader;
    int textureUnit;

    bool hasTexture = false;
    bool appears = true;

public:
    Object3D model;
    int mappingType = PLANAR_PROJECTION;

    ModelComponent(const char* filename, GLManager* glManager, std::string shader) :
        glManager(glManager),
        shader(shader)
    {
        bool triangulate = true;
        const char* basepath = NULL;
        auto objModel = new ObjModel(filename, basepath, triangulate);
        objModel->computeNormals();

        model = glManager->generateObject3D(objModel);

        delete(objModel);
    }

    ~ModelComponent()
    {
    }

    void init() override
    {
        std::cout << entity->name
            << ": bbox_min = (" << model.bbox_min.x
            << "," << model.bbox_min.y
            << "," << model.bbox_min.z
            << ")" << std::endl;
        std::cout << entity->name
            << ": bbox_max = (" << model.bbox_max.x
            << "," << model.bbox_max.y
            << "," << model.bbox_max.z
            << ")" << std::endl;

        entity->getComponent<TransformComponent>().yOffset = (float)std::abs(model.bbox_min.y);
    }

    void update() override
    {
        // glManager->setActiveShader(shader);
    }

    void show()
    {
        appears = true;
    }

    void hide()
    {
        appears = false;
    }

    void loadTexture(const char* filename)
    {
        textureUnit = glManager->LoadTextureImage(filename);
        hasTexture = true;
    }

    void draw() override
    {
        if(appears)
        {
            GLuint shaderID = glManager->shaders["default"];

            glm::mat4 modelTransf = entity->getComponent<TransformComponent>().getModelMatrix();
            glUniformMatrix4fv(glManager->model_uniform, 1 , GL_FALSE , glm::value_ptr(modelTransf));

            if(hasTexture)
            {
                glUniform1i(glGetUniformLocation(shaderID, "tex"), textureUnit);
                glUniform1i(glGetUniformLocation(shaderID, "hasTexture"), 1);
                glUniform1i(glGetUniformLocation(shaderID, "mappingType"), mappingType);
            }
            else
            {
                glUniform1i(glGetUniformLocation(shaderID, "hasTexture"), 0);
            }


            glManager->drawObject(model);
        }
    }
};