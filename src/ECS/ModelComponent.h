#pragma once
#include "Components.h"
#include "SDL2/SDL.h"
#include "../include/tiny_obj_loader.h"
#include "../GLManager.h"
#include <map>
#include <vector>
#include <glm/mat4x4.hpp>

class ModelComponent : public Component
{
private:
    Object3D model;
    GLManager* glManager;
    std::string shader;

public:
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
    }

    void update() override
    {
        glManager->setActiveShader(shader);
    }

    void draw() override
    {
        glm::mat4 modelTransf = entity->getComponent<TransformComponent>().getModelMatrix();
        glUniformMatrix4fv(glManager->model_uniform, 1 , GL_FALSE , glm::value_ptr(modelTransf));

        glManager->drawObject(model);
    }
};