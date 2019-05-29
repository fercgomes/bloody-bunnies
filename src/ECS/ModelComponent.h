#pragma once
#include "Components.h"
#include "SDL2/SDL.h"
#include "../include/tiny_obj_loader.h"
#include "../GLManager.h"
#include <map>
#include <vector>

class ModelComponent : public Component
{
private:
    Object3D model;
    GLManager* glManager;

public:

    ModelComponent(const char* filename, GLManager* glManager) :
        glManager(glManager)
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
    }

    void draw() override
    {
        /* precisa setar as matrizes model, view, blablabla */
        glManager->drawObject(model);
    }
};