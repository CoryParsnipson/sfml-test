#include "TestFSM.h"

TestFSM::TestFSM()
	: HasState("TestFSM")
	, test_data(12)
{
	// create states
	this->add_state("idle", [this]() { this->state_idle(); });
	this->add_state("state_1", [this]() { this->state_1(); });

	// add transitions
	this->add_transition("idle", "state_1", [this](HasState::INPUT input) {
		if (this->test_data == 12) {
			this->test_data = 21;
			std::cout << "  [transition: idle -> state 1]" << std::endl << std::endl;
			return true;
		}

		return false;
	});

	this->add_transition("state_1", "idle", [this](HasState::INPUT input) {
		if (this->test_data == 21) {
			this->test_data = 12;

			std::cout << "  [transition: state 1 -> idle]" << std::endl << std::endl;
			return true;
		}

		return false;
	});
}

void TestFSM::state_idle() {
	std::cout << this->name << ": idle" << std::endl;
}

void TestFSM::state_1() {
	std::cout << this->name << ": state 1" << std::endl;
}