#include "dependencies.h"
#include "Game.h"

#include "StartMenuScene.h"

int main()
{
   Game g;

   Scene* scene = new StartMenuScene();
   g.load_scene(scene);

   // start game!
   g.start();

	return 0;
}
