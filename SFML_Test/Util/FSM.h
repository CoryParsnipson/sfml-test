#ifndef FSM_H
#define FSM_H

#include "../dependencies.h"

class HasState {
protected:
	class State; // forward declaration of internal state class (see below)

public:
	enum INPUT {
		NOP,
		RESET,

		CANCEL,
	};

	typedef std::function<bool(HasState::INPUT)> transition_condition_fptr;

	HasState();
	virtual ~HasState() = 0; // abstract interface

	void add_state(std::string state_id, void(*state_func)());
	HasState::State* get_state(std::string state_id);

	void add_transition(
		std::string from_id,
		std::string to_id,
		transition_condition_fptr tc,
		unsigned int transition_probability = 1
	);

	// recommended external API
	void update(HasState::INPUT input);

protected:
	// ------------------------------------------------------------------------
	// FSM transition subclass
	// ------------------------------------------------------------------------
	class Transition {
	public:
		Transition(std::string from, std::string to, transition_condition_fptr tc, unsigned int prob);

		const std::string id_state_to;
		const std::string id_state_from;

	protected:
		unsigned int transition_probability;
		transition_condition_fptr tc;
	};

	// ------------------------------------------------------------------------
	// FSM state subclass
	// ------------------------------------------------------------------------
	class State {
	public:
		State(HasState* fsm, std::string state_id, void(*state_func)());

		void register_transition(HasState::Transition& t);
		bool comes_from(std::string state_id);
		bool goes_to(std::string state_id);

	protected:
		HasState* fsm; // pointer to parent FSM

		bool has_state(std::string state_id, std::list<HasState::State*>& l);

		std::string state_id;

		// func ptr containing actions to perform upon transition to this state
		void(*state_func)();

		std::list<HasState::State*> out; // list of states that this can transition to
		std::list<HasState::State*> in; // list of states that can transition to this
	};

	std::map<std::string, HasState::State*> states;
	std::map<std::string, std::list<HasState::Transition> > transitions;
};

// define destructor here, so everything works
inline HasState::~HasState() {
	std::map<std::string, HasState::State*>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); it++) {
		delete it->second;
	}
	this->states.clear();
}

#endif