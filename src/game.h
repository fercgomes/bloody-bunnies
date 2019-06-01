#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "GLManager.h"
#include <GLFW/glfw3.h>

class Game
{
public: 
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();
    static bool isRunning;

    enum groupLabels : std::size_t
    {
        testGroup,
        groupGrass
        // groupMap,
        // groupPlayers,
        // groupColliders,
        // groupProjectiles
    };

private:
    GLFWwindow *window;

    int count;
};