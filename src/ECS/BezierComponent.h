#pragma once
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

	void animate();

	void update();

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

	void setP(int p, glm::vec4 newp){
        switch(p){
            case 1:
                this->p1 = newp;
                break;
            case 2:
                this->p2 = newp;
                break;
            case 3:
                this->p3 = newp;
                break;
            case 4:
                this->p4 = newp;
                break;
        }
	}

	float getDuration(){
        return duration;
	}
};
