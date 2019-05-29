#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "GLManager.h"

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

    // static SDL_Renderer *renderer;
    static SDL_Event event;
    // static AssetManager* assets;

    // static SDL_Rect camera;

    enum groupLabels : std::size_t
    {
        testGroup
        // groupMap,
        // groupPlayers,
        // groupColliders,
        // groupProjectiles
    };

private:
    SDL_Window *window;
    SDL_GLContext context;

    int count;
};