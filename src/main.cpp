#include "game.h"

Game *game = nullptr;

const char* gameName = "The Amazing FCG Game";
const int width = 800;
const int height = 600;

int main( int argc, char* args[] )
{
	double prevTime, thisTime;

	game = new Game();
	game->init(gameName, width, height);

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