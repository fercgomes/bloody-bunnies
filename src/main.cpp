#include "game.h"

Game *game = nullptr;

const char* gameName = "The Amazing FCG Game";
const float screenRatio = 16.0f/9.0f;
	const int resolution = 720;

int main( int argc, char* args[] )
{
	double prevTime, thisTime;

	game = new Game();
	game->init(gameName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenRatio * resolution, resolution, false);

	prevTime = glfwGetTime();

	while(game->running())
	{
		thisTime = glfwGetTime();
		Game::dt = thisTime - prevTime;
		prevTime = glfwGetTime();

		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();

	return 0;
}