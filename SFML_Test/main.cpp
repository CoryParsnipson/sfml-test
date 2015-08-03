#include "dependencies.h"
#include "Game.h"

#include "Entities/TestFSM.h"

int main()
{
	//Game* g = new Game();

	TestFSM* fsm = new TestFSM();

	fsm->on();
	fsm->update(HasState::INPUT::NOP);
	fsm->update(HasState::INPUT::NOP);
	fsm->update(HasState::INPUT::NOP);
	fsm->update(HasState::INPUT::NOP);
	fsm->update(HasState::INPUT::NOP);

	return 0;
}