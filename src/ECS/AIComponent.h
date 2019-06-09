#pragma once
#include "Components.h"
#include "../game.h"
#include <glm/vec4.hpp>

class AIComponent : public Component
{
private:
    TransformComponent* playerTransform;

    float actionDuration;

public:

    AIComponent(TransformComponent* player)
    {
        playerTransform = player;
    }

    /* Generates a random float in the interval [a, b] */
    float randFloat(float a, float b)
    {
        return a + static_cast<float>(rand()) / ( static_cast <float> ( RAND_MAX / (b - a) ) );
    }

    void init() override
    {
        /* AIComponent needs a TransformComponent */
        if(entity->hasComponent<TransformComponent>() == false)
        {
            entity->addComponent<TransformComponent>();
        }
    }

    void update() override
    {
        auto& AITransform = entity->getComponent<TransformComponent>();

        /* Vector pointing to player */
        // Vector3D toPlayer(playerTransform->position - AITransform->position);
        glm::vec4 toPlayer = glm::normalize(glm::vec4(
            playerTransform->position.x - AITransform.position.x,
            playerTransform->position.y - AITransform.position.y,
            playerTransform->position.z - AITransform.position.z,
            1.0f
        ));

        float distanceToPlayer = norm(glm::vec4(
            playerTransform->position.x - AITransform.position.x,
            playerTransform->position.y - AITransform.position.y,
            playerTransform->position.z - AITransform.position.z,
            1.0f
        ));

        float AISpeed = 1.0f;

        /* Ongoing action */
        if(actionDuration > 0.0f)
        {
            float minDistance = 6.0f;

            if(distanceToPlayer < minDistance)
            {
                AITransform.velocity.x = AISpeed * toPlayer.x;
                AITransform.velocity.z = AISpeed * toPlayer.z;
            }
            else
            {
                AITransform.velocity.x = 0.0f;
                AITransform.velocity.z = 0.0f;
            }
            

            actionDuration -= Game::dt;
        }
        else
        {
            actionDuration = randFloat(1.0f, 3.0f);
        }
        

    }

};