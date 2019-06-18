#pragma once
#include "../Camera.h"
#include "../game.h"
#include "ECS.h"
#include "Components.h"
#include "../Vector3D.h"

class KeyboardController : public Component
{

public:
    TransformComponent *transform;
    Camera *camera;

    glm::vec4 mouseDirection;

    bool goingForward = false;
    bool goingBackwards = false;
    bool goingRight = false;
    bool goingLeft = false;
    bool jump = false;

    float speed = 4.0f;

    KeyboardController(Camera *cam) : camera(cam)
    {}

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        glm::vec4 velVec = glm::normalize(
            glm::vec4(camera->viewVector.x, 0.0f, camera->viewVector.z, 0.0f)
        );

        mouseDirection.x = velVec.x;
        mouseDirection.z = velVec.z;
        mouseDirection.y = velVec.y;
        mouseDirection.w = 0.0f;

        glm::vec3 newVelocity(0.0f, transform->velocity.y, 0.0f);

        glm::vec4 up(0.0f, 1.0f, 0.0f, 0.0f);
        glm::vec4 rightVector = crossproduct(mouseDirection, up);

        /* If the player jumped is not already on air */
        if(jump && !transform->onAir)
        {
            float jumpSpeed = 10.0f;
            newVelocity.y = jumpSpeed;
            transform->onAir = true;
        }

        if(goingForward)
        {
            newVelocity.x += speed * mouseDirection.x;
            newVelocity.z += speed * mouseDirection.z;
        }

        if(goingBackwards)
        {
            newVelocity.x += speed * - mouseDirection.x;
            newVelocity.z += speed * - mouseDirection.z;
        }

        if(goingRight)
        {
            newVelocity.x += speed * rightVector.x;
            newVelocity.z += speed * rightVector.z;
        }

        if(goingLeft)
        {
            newVelocity.x += speed * -rightVector.x;
            newVelocity.z += speed * -rightVector.z;
        }

        /* We can only jump once */
        jump = false;

        transform->velocity.x = newVelocity.x;
        transform->velocity.y = newVelocity.y;
        transform->velocity.z = newVelocity.z;
    }


};
