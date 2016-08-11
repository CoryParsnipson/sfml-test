#include "dependencies.h"
#include "Game.h"

#include "StartMenuScene.h"

int main()
{
   // initialize game instance
   Game::instance(); // TODO: This is BAD. Introducing static intialization dependencies with other singletons here...

   // don't need to delete scene objects given to game (messy, I know)
   Game::instance()->load_scene(new StartMenuScene());
   Game::instance()->start();

	return 0;
}
