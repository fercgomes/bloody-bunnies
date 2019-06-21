#pragma once
#include <string>
#include "Components.h"
#include "../game.h"
#include <glm/vec4.hpp>

typedef struct bbox{
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
}BoundingBox;

class TransformComponent;
class ModelComponent;
class AIComponent;
class ColliderComponent : public Component
{

private:
    BoundingBox boundingbox;
    bool initializedBoundingBox;
    bool isSphere;
    float sphereRadius;

public:
    std::string tag;

    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
        isSphere = false;
    }

    ColliderComponent(std::string t, BoundingBox box)
    {
        tag = t;
        this->boundingbox = box;
        this->initializedBoundingBox = true;
        isSphere = false;
    }

    ColliderComponent(std::string t, bool sphere, float radius)
    {
        tag = t;
        this->isSphere = true;
        this->sphereRadius = radius;
        this->initializedBoundingBox = true;
    }

    BoundingBox getBoundingBox(){
        return this->boundingbox;
    }

    void init() override;

    void update() override
    {

    }

    bool isAsphere(){return this->isSphere;}

    float getSphereRadius(){return this->sphereRadius;}

    bool isColliding();

    //Checks if we are colliding with targetEntity
    bool checkAgainst(Entity& targetEntity);

};
