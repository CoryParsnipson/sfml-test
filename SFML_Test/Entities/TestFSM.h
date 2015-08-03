#ifndef TEST_FSM_H
#define TEST_FSM_H

#include "../Util/FSM.h"

class TestFSM : public HasState {
public:
	TestFSM();
protected:
	int test_data;

	void state_idle();
	void state_1();
};

#endif