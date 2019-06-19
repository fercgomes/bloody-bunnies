#pragma once
#include "Components.h"

//Destroys entity after some time
class AutoKillComponent : public Component
{

private:
    double maxLivingTime, currentLivingTime;

public:

    AutoKillComponent(double timeOut)
    {
        this->maxLivingTime = timeOut;
        this->currentLivingTime = 0.0f;
    }

    void update() override
    {
        this->currentLivingTime = this->currentLivingTime + Game::dt;

        if(this->currentLivingTime >= this->maxLivingTime)
            entity->destroy();
    }

};
