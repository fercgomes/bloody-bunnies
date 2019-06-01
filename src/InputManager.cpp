#include "game.h"
#include "ECS/ECS.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
    Game* game;
    Manager* manager;
    
    void readKeys(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(key == GLFW_KEY_W && action == GLFW_PRESS);
    }
};