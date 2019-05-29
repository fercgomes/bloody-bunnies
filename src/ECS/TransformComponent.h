#pragma once
#include "Components.h"
#include <glm/vec4.hpp>
#include "../Vector3D.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include "../include/matrices.h"

#define GRAV 0.01f

class TransformComponent : public Component
{
public:
    Vector3D position;
    Vector3D velocity;
    glm::mat4 modelMatrix;

    float speed = 1.1f;

    TransformComponent()
    {
        position = Vector3D(0.0f, 0.0f, 0.0f);
        velocity = Vector3D(0.0f, 0.0f, 0.0f);
    }

    TransformComponent(float x, float y, float z)
    {
        position.x = x;
        position.y = y;
        position.z = y;
    }

    glm::mat4 getModelMatrix()
    {
        return modelMatrix * Matrix_Scale(0.5f, 0.5f, 0.5f);
    }

    void init() override
    {
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
        position.z += velocity.z * speed;

        if(position.y > 0)
            velocity.y -= GRAV;
        if(position.y <= 0)
            velocity.y = 0;

        modelMatrix = Matrix_Identity() * Matrix_Translate(position.x, position.y, position.z);
    }
};