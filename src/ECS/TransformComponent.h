#pragma once
#include "Components.h"
#include <glm/vec4.hpp>
#include "../Vector3D.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include "../include/matrices.h"

#define PI 3.14

#define GRAV 0.01f

class TransformComponent : public Component
{
public:
    Vector3D position;
    Vector3D velocity;
    Vector3D lookingAt;

    glm::mat4 modelMatrix;
    bool onAir = false;
    bool fixed = false;

    float x_Rotation;
    float y_Rotation;
    float z_Rotation;
    float x_Scale;
    float y_Scale;
    float z_Scale;

    float speed = 1.1f;

    TransformComponent()
    {
        position = Vector3D(0.0f, 0.0f, 0.0f);
        velocity = Vector3D(0.0f, 0.0f, 0.0f);
        x_Rotation = 0.0f;
        y_Rotation = 0.0f;
        z_Rotation = 0.0f;
        x_Scale = 1.0f;
        y_Scale = 1.0f;
        z_Scale = 1.0f;
    }

    TransformComponent(float x, float y, float z)
    {
        position.x = x;
        position.y = y;
        position.z = y;
        x_Rotation = 0.0f;
        y_Rotation = 0.0f;
        z_Rotation = 0.0f;
        x_Scale = 1.0f;
        y_Scale = 1.0f;
        z_Scale = 1.0f;
    }

    void setStuff(float rX, float rY, float rZ, float sX, float sY, float sZ)
    {
        this->x_Rotation = rX;
        this->y_Rotation = rY;
        this->z_Rotation = rZ;
        this->x_Scale = sX;
        this->y_Scale = sY;
        this->z_Scale = sZ;
    }

    glm::mat4 getModelMatrix()
    {
        return modelMatrix;
    }

    glm::vec4 getPos() const { return glm::vec4(position.x, position.y, position.z, 1.0f); }

    void init() override
    {
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
        position.z += velocity.z * speed;

        if(!fixed)
        {
            if(position.y > 0)
                velocity.y -= GRAV;
            if(position.y <= 0)
            {
                onAir = false;
                velocity.y = 0;
            }
        }

        modelMatrix = Matrix_Identity()
                    * Matrix_Translate(position.x, position.y, position.z)
                    * Matrix_Scale(x_Scale, y_Scale, z_Scale)
                    * Matrix_Rotate_X(this->x_Rotation)
                    * Matrix_Rotate_Y(this->y_Rotation)
                    * Matrix_Rotate_Z(this->z_Rotation);
    }
};
