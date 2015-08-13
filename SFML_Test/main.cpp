#include "dependencies.h"
#include "Game.h"

#include "Entities/TestFSM.h"

int main()
{
	Game* g = new Game();
   g->reset(); // start game, automatically enters game loop

	return 0;
}
