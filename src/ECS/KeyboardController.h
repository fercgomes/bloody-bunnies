#pragma once
#include "../game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{

public:
    TransformComponent *transform;


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
    }

};
