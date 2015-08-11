#include "dependencies.h"
#include "Game.h"

#include "Entities/TestFSM.h"

int main()
{
	Game* g = new Game();
   g->loop();
   
	/*TestFSM* fsm = new TestFSM();

	fsm->on();
	fsm->update(0);
	fsm->update(0);
	fsm->update(0);
	fsm->update(0);
	fsm->update(0);
   */
	return 0;
}
