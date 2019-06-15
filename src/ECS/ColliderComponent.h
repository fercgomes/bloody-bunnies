#pragma once
#include <string>
#include "Components.h"
#include <glm/vec4.hpp>

typedef struct bbox{
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
}BoundingBox;

class TransformComponent;
class ModelComponent;
class ColliderComponent : public Component
{

private:
    BoundingBox boundingbox;
    bool initializedBoundingBox;

public:
    std::string tag;

    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    ColliderComponent(std::string t, BoundingBox box)
    {
        tag = t;
        this->boundingbox = box;
        this->initializedBoundingBox = true;
    }

    BoundingBox getBoundingBox(){
        return this->boundingbox;
    }

    void init() override;

    void update() override
    {

    }

    bool isColliding(){
        auto& myGroup = entity->getManager().getGroup(0); //TODO: getGroup(0) is very dumb

        for(auto& e : myGroup){
            if(e != entity && this->checkAgainst(*e)){
                printf("%s collided with %s\n", entity->name.c_str(), e->name.c_str());
                return true;
            }
        }

        return false;
    }

    //Checks if we are colliding with targetEntity
    bool checkAgainst(Entity& targetEntity);

};
