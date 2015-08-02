#include "dependencies.h"
#include "Game.h"

#include "Entities/TestFSM.h"

int main()
{
	//Game* g = new Game();

	TestFSM* fsm = new TestFSM();

	//FSM::State& state = fsm->get_state("from");
	/*
	auto tc = [=](FSM::INPUT input) {
		std::cout << "I AM LAMBDA FUNK: " << std::endl;

		fsm->get_state("from");
		return FSM::INPUT::NOP == input;
	};

	fsm->add_transition("from", "to", tc);
	*/
	/* -- psuedo code for FSM --

	FSM* fsm = new FSM();

	fsm.add_state("state1");
	fsm.add_state("state2");
	fsm.add_state("state3");
	fsm.add_state("state4");

	fsm.add_transition(from_state, to_state, condition, probability) ? ;

	fsm.start();
	fsm.get_current_state();

	output = fsm.update(input condition); // given input compute next transition and output

	// -- end pseudo code for FSM -- */

	return 0;
}