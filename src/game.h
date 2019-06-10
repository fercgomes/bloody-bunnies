#pragma once
#include <iostream>
#include <vector>
#include "GLManager.h"
#include <GLFW/glfw3.h>

class Game
{
public: 
    Game();
    ~Game();

    void init(const char* title, int width, int height);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running();
    static bool isRunning;

    static double dt;
    static float screenRatio;

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