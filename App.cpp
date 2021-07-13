#include "Game.h"

int main(int argc, char** args)
{
	Game game;

	if (game.IsInit())
		game.Run();

	return 0;
}