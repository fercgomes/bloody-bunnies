#pragma once
#include <string>
#include "Components.h"
#include <glm/vec4.hpp>

typedef struct bbox{
    float minX, minY, minZ;
    float maxX, maxY, maxZ;
}BoundingBox;

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

    void init() override
    {
        /* makes sure we have a transform component available */
        if(!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();

        if(!this->initializedBoundingBox){
            if(entity->hasComponent<ModelComponent>()){
                ModelComponent& modelC = entity->getComponent<ModelComponent>();
                this->boundingbox.minX = modelC.model.bbox_min.x * transform->x_Scale + transform->xOffset;
                this->boundingbox.minY = modelC.model.bbox_min.y * transform->y_Scale + transform->yOffset;
                this->boundingbox.minZ = modelC.model.bbox_min.z * transform->z_Scale + transform->zOffset;
                this->boundingbox.maxX = modelC.model.bbox_max.x * transform->x_Scale + transform->xOffset;
                this->boundingbox.maxY = modelC.model.bbox_max.y * transform->y_Scale + transform->yOffset;
                this->boundingbox.maxZ = modelC.model.bbox_max.z * transform->z_Scale + transform->zOffset;
                this->initializedBoundingBox = true;
            }
            else{
                this->boundingbox.maxX = 1;
                this->boundingbox.minX = -1;
                this->boundingbox.maxY = 1;
                this->boundingbox.minY = -1;
                this->boundingbox.maxZ = 1;
                this->boundingbox.minZ = -1;
            }
            this->initializedBoundingBox = true;
        }
    }

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
    bool checkAgainst(Entity& targetEntity){
        //We cant collide against an entity without ColliderComponent
        if(!targetEntity.hasComponent<ColliderComponent>())
            return false;

        BoundingBox targetBB = targetEntity.getComponent<ColliderComponent>().getBoundingBox();
        BoundingBox myTempBB = this->getBoundingBox();
        TransformComponent targetTransform = targetEntity.getComponent<TransformComponent>();

        targetBB.maxX = targetBB.maxX + targetTransform.getPos().x;
        targetBB.minX = targetBB.minX + targetTransform.getPos().x;
        targetBB.maxY = targetBB.maxY + targetTransform.getPos().y;
        targetBB.minY = targetBB.minY + targetTransform.getPos().y;
        targetBB.maxZ = targetBB.maxZ + targetTransform.getPos().z;
        targetBB.minZ = targetBB.minZ + targetTransform.getPos().z;

        myTempBB.maxX = myTempBB.maxX + transform->getPos().x;
        myTempBB.minX = myTempBB.minX + transform->getPos().x;
        myTempBB.maxY = myTempBB.maxY + transform->getPos().y;
        myTempBB.minY = myTempBB.minY + transform->getPos().y;
        myTempBB.maxZ = myTempBB.maxZ + transform->getPos().z;
        myTempBB.minZ = myTempBB.minZ + transform->getPos().z;

        if(myTempBB.maxX > targetBB.minX && myTempBB.maxY > targetBB.minY && myTempBB.maxZ > targetBB.minZ){
            if(targetBB.maxX > myTempBB.minX && targetBB.maxY > myTempBB.minY && targetBB.maxZ > myTempBB.minZ)
                return true;
        }
        /* OBS: Checking twice, once for each entity... */
        else if(targetBB.maxX > myTempBB.minX && targetBB.maxY > myTempBB.minY && targetBB.maxZ > myTempBB.minZ){
            if(myTempBB.maxX > targetBB.minX && myTempBB.maxY > targetBB.minY && myTempBB.maxZ > targetBB.minZ)
                return true;
        }

        return false;
    }

};
