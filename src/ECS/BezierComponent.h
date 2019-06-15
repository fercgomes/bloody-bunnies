#include "../game.h"
#include "Components.h"
#include "ColliderComponent.h"

class BezierComponent : public Component
{
private:
	glm::vec4 p1_relative, p2_relative, p3_relative, p4_relative;
	glm::vec4 p1, p2, p3, p4;
	float currentTime = 0.0f;
	float duration;

	bool running = true;

public:
	BezierComponent(float dur, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4)
	{
		duration = dur;
		this->p1 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		this->p2 = p2;
		this->p3 = p3;
		this->p4 = p4;
		running = false;
	}

	void animate()
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

	void update() override
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
                if(entity->hasComponent<ColliderComponent>()){
                    if(entity->getComponent<ColliderComponent>().isColliding()){
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

	glm::vec4 getPosition()
	{
		float t = currentTime / duration;

		glm::vec4 c12 = p1_relative + t * (p2_relative - p1_relative);
		glm::vec4 c23 = p2_relative + t * (p3_relative - p2_relative);
		glm::vec4 c34 = p3_relative + t * (p4_relative - p3_relative);
		glm::vec4 c123 = c12 + t * (c23 - c12);
		glm::vec4 c234 = c23 + t * (c34 - c23);
		glm::vec4 result = c123 + t * (c234 - c123);

		return result;
	}
};
