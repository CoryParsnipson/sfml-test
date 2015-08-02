#include "TestFSM.h"

TestFSM::TestFSM()
	: test_data(12)
{
	// create states
	this->add_state("idle", []() {
		std::cout << "ENTERING IDLE STATE" << std::endl;
	});

	this->add_state("state_1", []() {
		std::cout << "ENTERING STATE 1" << std::endl;
	});

	// add transitions
	this->add_transition("idle", "state_1", [this](HasState::INPUT input) {
		std::cout << "Testing for Transition between IDLE and STATE 1..." << std::endl;

		if (this->test_data == 12) {
			this->test_data = 21;
			std::cout << "IDLE -> STATE 1" << std::endl;
			return true;
		}

		return false;
	});

	this->add_transition("state_1", "idle", [this](HasState::INPUT input) {
		std::cout << "Testing for Transition between STATE 1 and IDLE..." << std::endl;

		if (this->test_data == 21) {
			this->test_data = 12;

			std::cout << "STATE 1 -> IDLE" << std::endl;
			return true;
		}

		return false;
	});
}