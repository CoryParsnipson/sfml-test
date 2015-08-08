#ifndef FSM_H
#define FSM_H

#include "../dependencies.h"

class HasState {
protected:
	class State; // forward declaration of internal state class (see below)

public:
	// ------------------------------------------------------------------------
	// external API (!!)
	// ------------------------------------------------------------------------
	void set_init_state(std::string init_state_id);

	void on();
	void off();
	virtual void reset();

	void update(int input);

	const std::string name;

	// ------------------------------------------------------------------------
	// constructors and destructors
	// ------------------------------------------------------------------------
	HasState(std::string name);
	virtual ~HasState() = 0; // abstract interface

protected:
	typedef std::function<bool(int)> transition_condition_fptr;

	// ------------------------------------------------------------------------
	// FSM transition subclass
	// ------------------------------------------------------------------------
	class Transition {
	public:
		static unsigned int next_id;

		// constructors
		Transition(std::string from, std::string to, transition_condition_fptr tc, unsigned int prob);

		// public methods
		bool will_transition(int input);
		unsigned int get_probability();

		// public members
		const unsigned int id;
		const std::string id_state_from;
		const std::string id_state_to;

	protected:
		unsigned int prob;
		transition_condition_fptr tc;
	};

	// ------------------------------------------------------------------------
	// FSM state subclass
	// ------------------------------------------------------------------------
	class State {
	public:
		State(HasState* fsm, std::string state_id, std::function<void()> state_func);

		void register_transition(HasState::Transition* t);
		bool comes_from(std::string state_id);
		bool goes_to(std::string state_id);

		bool has_transition_in(HasState::Transition* t);
		bool has_transition_in(unsigned int id);

		bool has_transition_out(HasState::Transition* t);
		bool has_transition_out(unsigned int id);

		// func ptr containing actions to perform upon transition to this state
		const std::function<void()> state_func;

		// given input, calculate the next state to transition to
		HasState::State* transition(int input);

	protected:
		bool has_transition(unsigned int id, std::list<HasState::Transition*>& l);
		bool has_state(std::string state_id, std::list<HasState::State*>& l);

		HasState* fsm; // pointer to parent FSM

		std::string state_id;

		std::list<HasState::State*> state_to;
		std::list<HasState::State*> state_from;

		std::list<HasState::Transition*> transitions_out;
		std::list<HasState::Transition*> transitions_in;
	};

	// ------------------------------------------------------------------------
	// internal functions to define states and transitions
	// ------------------------------------------------------------------------
	void add_state(std::string state_id, std::function<void()> state_func);
	HasState::State* get_state(std::string state_id);

	void add_transition(
		std::string from_id,
		std::string to_id,
		transition_condition_fptr tc,
		unsigned int transition_probability = 1
		);

	// ------------------------------------------------------------------------
	// HasState member functions
	// ------------------------------------------------------------------------
	bool is_on;
	std::string init_state_id;
	HasState::State* current_state;

	// master transition and state registries
	std::list<HasState::Transition*> transitions;
	std::map<std::string, HasState::State*> states;
};

#endif
