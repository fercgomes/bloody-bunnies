#pragma once
#include "Components.h"
#include "../game.h"
#include <glm/vec4.hpp>

class AIComponent : public Component
{
private:
    TransformComponent* playerTransform;

    float actionDuration;
    bool jumping;

public:

    AIComponent(TransformComponent* player)
    {
        playerTransform = player;
        jumping = false;
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

        float AISpeed = 2.0f;

        /* Ongoing action */
        if(actionDuration > 0.0f)
        {
            if(!jumping){
                float minDistance = 70.0f;

                if(distanceToPlayer < minDistance)
                {
                    AITransform.velocity.x = AISpeed * toPlayer.x;
                    AITransform.velocity.z = AISpeed * toPlayer.z;
                    AITransform.y_Rotation = dot(toPlayer, glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
                }
                else
                {
                    AITransform.velocity.x = 0.0f;
                    AITransform.velocity.z = 0.0f;
                }
            }

            actionDuration -= Game::dt;
        }
        else
        {
            //TODO: It is incredible that this works...
            if(rand()%5 < 3 && entity->hasComponent<BezierComponent>()){
                switch(rand()%4){
                    case 0:
                        entity->getComponent<BezierComponent>().setP(2, glm::vec4(2.0f, 10.0f, 0.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(3, glm::vec4(10.0f, 10.0f, 0.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(4, glm::vec4(12.0f, 0.0f, 0.0f, 1.0f));
                        break;
                    case 1:
                        entity->getComponent<BezierComponent>().setP(2, glm::vec4(-2.0f, 10.0f, 0.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(3, glm::vec4(-10.0f, 10.0f, 0.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(4, glm::vec4(-12.0f, 0.0f, 0.0f, 1.0f));
                        break;
                    case 2:
                        entity->getComponent<BezierComponent>().setP(2, glm::vec4(0.0f, 10.0f, 2.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(3, glm::vec4(0.0f, 10.0f, 10.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(4, glm::vec4(0.0f, 0.0f, 12.0f, 1.0f));
                        break;
                    case 3:
                        entity->getComponent<BezierComponent>().setP(2, glm::vec4(0.0f, 10.0f, -2.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(3, glm::vec4(0.0f, 10.0f, -10.0f, 1.0f));
                        entity->getComponent<BezierComponent>().setP(4, glm::vec4(0.0f, 0.0f, -12.0f, 1.0f));
                        break;
                }
                entity->getComponent<BezierComponent>().animate();
                jumping = true;
                actionDuration = entity->getComponent<BezierComponent>().getDuration() + 0.2f;
            }
            else{
                jumping = false;
                actionDuration = randFloat(1.0f, 3.0f);
            }
        }


    }

};
