#pragma once
#include "../game.h"
#include "ECS.h"
#include "Components.h"
#include "../Audio.cpp"

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
        if(Game::event.type == SDL_KEYDOWN)
        {
            switch(Game::event.key.keysym.sym)
            {
                case SDLK_w:
                    transform->velocity.z = 0.2f;
                    break;
                
                case SDLK_a:
                    transform->velocity.x = 0.2f;
                    break;

                case SDLK_d:
                    transform->velocity.x = -0.2f;
                    break;
                
                case SDLK_s:
                    transform->velocity.z = -0.2f;
                    break;
                
                case SDLK_SPACE:
                    transform->velocity.y = 0.3f;
                    Audio::play("../jump.wav");
                    break;

                default:
                    break;
            }
        
        }

        if(Game::event.type == SDL_KEYUP)
        {
            switch(Game::event.key.keysym.sym)
            {
                case SDLK_w:
                    transform->velocity.z = 0;
                    break;
                
                case SDLK_a:
                    transform->velocity.x = 0;
                    break;

                case SDLK_d:
                    transform->velocity.x = 0;
                    break;
                
                case SDLK_s:
                    transform->velocity.z = 0;
                    break;
                
                case SDLK_ESCAPE:
                    Game::isRunning = false;

                default:
                    break;
            }
        }
    }

};
