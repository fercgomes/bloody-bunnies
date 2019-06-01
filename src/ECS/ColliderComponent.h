#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{

public:
    std::string tag;

    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    ColliderComponent(std::string t, int xpos, int ypos, int size)
    {
        tag = t;
    }

    void init() override
    {
        /* makes sure we have a transform component available */
        if(!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
    }

};