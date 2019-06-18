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

    void addEnemy(double x, double y, double z);
    void throwRock();

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
        groupGrass,
        mapGroup
        // groupMap,
        // groupPlayers,
        // groupColliders,
        // groupProjectiles
    };

private:
    GLFWwindow *window;

    int count;
};
