#include "ModelComponent.h"
#include "TransformComponent.h"
#include "BezierComponent.h"

void BezierComponent::animate()
{
    if(!running)
    {
        running = true;
        p1_relative = p1 + entity->getComponent<TransformComponent>().getPos();
        p2_relative = p2 + entity->getComponent<TransformComponent>().getPos();
        p3_relative = p3 + entity->getComponent<TransformComponent>().getPos();
        p4_relative = p4 + entity->getComponent<TransformComponent>().getPos();
    }
}

void BezierComponent::update()
	{
		if(running)
		{
			currentTime += Game::dt;

			if(currentTime <= duration)
			{
				auto& transf = entity->getComponent<TransformComponent>();
				auto prevPos = transf.position;
				auto newPos = getPosition();

				transf.position.x = newPos.x;
				transf.position.y = newPos.y;
				transf.position.z = newPos.z;

				//Check collision
				bool outOfMap = false;
                if(transf.position.x >= 47.0f || transf.position.x <= -47.0f || transf.position.z >= 47.0f || transf.position.z <= -47.0f)
                    outOfMap = true;

                if(entity->hasComponent<ColliderComponent>() || outOfMap){
                    if(outOfMap || entity->getComponent<ColliderComponent>().isColliding()){
                        transf.position = prevPos;
                        running = false;
                        currentTime = 0.0f;
                        return;
                    }
                }
			}
			else
			{
				running = false;
				currentTime = 0.0f;
			}
		}
	}
