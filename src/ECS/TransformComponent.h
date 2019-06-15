#pragma once
#include "Components.h"
#include "ColliderComponent.h"
#include <glm/vec4.hpp>
#include "../Vector3D.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec4.hpp>
#include "../include/matrices.h"
#include "../game.h"

#define PI 3.14

#define GRAV_FORCE 9.8f

class TransformComponent : public Component
{
public:
    Vector3D position;
    Vector3D velocity;

	float torque = 1.0f;

    bool onAir = false;
    bool fixed = false;

    float x_Rotation;
    float y_Rotation;
    float z_Rotation;

    float x_Scale;
    float y_Scale;
    float z_Scale;

    float xOffset = 0.0f;
    float yOffset = 0.0f;
    float zOffset = 0.0f;

    float moving = 0.0f;
    float velAmount = 4.0f;

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

        this->yOffset = this->yOffset * sY;
    }

    glm::mat4 getModelMatrix()
    {
        return        Matrix_Identity()
                    * Matrix_Translate(position.x + xOffset, position.y + yOffset, position.z + zOffset)
                    * Matrix_Scale(x_Scale, y_Scale, z_Scale)
                    * Matrix_Rotate_X(this->x_Rotation)
                    * Matrix_Rotate_Y(this->y_Rotation)
                    * Matrix_Rotate_Z(this->z_Rotation);
    }

    glm::vec4 getPos() const { return glm::vec4(position.x, position.y, position.z, 1.0f); }


    void init() override
    {
    }

    void update() override
    {

        position.x += velocity.x * Game::dt;
        position.y += velocity.y * Game::dt;
        position.z += velocity.z * Game::dt;

        if(entity->hasComponent<ColliderComponent>() && entity->getComponent<ColliderComponent>().isColliding()){
            onAir = false;
            position.x -= velocity.x * Game::dt;
            position.y -= velocity.y * Game::dt;
            position.z -= velocity.z * Game::dt;
            velocity.y = 0;
            velocity.x = 0;
            velocity.z = 0;
            return;
        }

		//y_Rotation += torque * Game::dt;

        if(!fixed)
        {

            /* Avoid falling into negative Y */
            if(position.y <= 0)
            {
                onAir = false;
                velocity.y = 0;
                position.y = 0;
            }

            /* Apply gravitational force */
            if(position.y > 0){
                velocity.y -= GRAV_FORCE * Game::dt;
            }
        }


    }
};
