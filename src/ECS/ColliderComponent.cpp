#include "ModelComponent.h"
#include "TransformComponent.h"

void ColliderComponent::init()
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

bool ColliderComponent::isColliding(){
        auto& myGroup = entity->getManager().getGroup(0); //TODO: getGroup(0) is very dumb
        if(this->isAsphere())
            return false;

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
                else if(entity->name == "rock" && e->name == "Balloon"){
                    Game::addRockToPick(e->getComponent<TransformComponent>().getPos().x, e->getComponent<TransformComponent>().getPos().y, e->getComponent<TransformComponent>().getPos().z);
                    Game::addRockToPick(e->getComponent<TransformComponent>().getPos().x, e->getComponent<TransformComponent>().getPos().y + 4.0f, e->getComponent<TransformComponent>().getPos().z);
                    Game::addRockToPick(e->getComponent<TransformComponent>().getPos().x, e->getComponent<TransformComponent>().getPos().y + 8.0f, e->getComponent<TransformComponent>().getPos().z);
                    e->destroy();
                }
                return true;
            }
        }

        return false;
    }

bool ColliderComponent::checkAgainst(Entity& targetEntity)
{
    //We cant collide against an entity without ColliderComponent
    if(!targetEntity.hasComponent<ColliderComponent>())
        return false;

    if(targetEntity.getComponent<ColliderComponent>().isAsphere()){
        BoundingBox myBB = this->getBoundingBox();
        glm::vec4 centralPoint((myBB.maxX + myBB.minX) / 2, (myBB.maxY + myBB.minY) / 2, (myBB.maxZ + myBB.minZ) / 2, 1.0f);
        centralPoint = centralPoint + transform->getPos() + glm::vec4(transform->xOffset, transform->yOffset, transform->zOffset, 1.0f);

        TransformComponent targetTransform = targetEntity.getComponent<TransformComponent>();

        if(norm(centralPoint - targetTransform.getPos()) < targetEntity.getComponent<ColliderComponent>().getSphereRadius())
            return true;
        else
            return false;
    }

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
