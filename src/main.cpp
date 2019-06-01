#include "game.h"

Game *game = nullptr;

const char* gameName = "The Amazing FCG Game";
const float screenRatio = 16.0f/9.0f;
	const int resolution = 720;

int main( int argc, char* args[] )
{
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init(gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenRatio * resolution, resolution, false);

	while(game->running())
	{
		frameStart = SDL_GetTicks();


		game->handleEvents();
		game->update();
		game->render();

		// frameTime = SDL_GetTicks() - frameStart;
		// if(frameDelay > frameTime)
		// {
		// 	SDL_Delay(frameDelay - frameTime);
		// }
	}

	game->clean();

	return 0;
}