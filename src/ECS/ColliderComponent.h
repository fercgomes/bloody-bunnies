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
                if(entity->name == "rock" && e->hasComponent<AIComponent>()){ //TODO: name == "rock" is also dumb
                    entity->destroy();
                    e->destroy();
                    Game::enemiesDefeated++;
                }
                else if(entity->name == "player" && e->name == "rockPickable"){
                    e->destroy();
                    printf("Player picked a Rock\n");
                    Game::playerAmmo++;
                }
                else if(entity->name == "Enemy" && e->name == "player"){
                    Game::gameLost = true;
                }
                return true;
            }
        }

        return false;
    }

    //Checks if we are colliding with targetEntity
    bool checkAgainst(Entity& targetEntity);

};
