#include "dependencies.h"
#include "Game.h"

#include "StartMenuScene.h"

int main()
{
   Game g;

   // don't need to delete scene objects given to game (messy, I know)
   g.load_scene(new StartMenuScene());
   g.start();

	return 0;
}
