#include "FSM.h"

HasState::Transition::Transition(std::string from, std::string to, transition_condition_fptr tc, unsigned int prob)
	: id_state_from(from)
	, id_state_to(to)
	, transition_probability(prob)
	, tc(tc)
{
}

HasState::State::State(HasState* fsm, std::string state_id, void(*state_func)())
	: fsm(fsm)
	, state_id(state_id)
	, state_func(state_func)
{
}

void HasState::State::register_transition(HasState::Transition& t) {
	if (this->state_id.compare(t.id_state_from) == 0) {
		// add state_to to list of out states (if not already)
		if (!this->goes_to(t.id_state_to)) {
			this->out.push_back(this->fsm->get_state(t.id_state_to));
		}
	}

	if (this->state_id.compare(t.id_state_to) == 0) {
		// add state_from to list of in states (if not already)
		if (!this->comes_from(t.id_state_from)) {
			this->in.push_back(this->fsm->get_state(t.id_state_from));
		}
	}

	// TODO: warning or exception on no match for both from and to?
}

bool HasState::State::comes_from(std::string state_id) {
	return this->has_state(state_id, this->in);
}

bool HasState::State::goes_to(std::string state_id) {
	return this->has_state(state_id, this->out);
}

bool HasState::State::has_state(std::string state_id, std::list<HasState::State*>& l) {
	std::list<HasState::State*>::iterator it;
	for (it = l.begin(); it != l.end(); it++) {
		if ((*it)->state_id.compare(this->state_id) == 0) {
			return true;
		}
	}
	return false;
}

HasState::HasState() {
}

void HasState::add_state(std::string state_id, void (*state_func)()) {
	HasState::State* s = new HasState::State(this, state_id, state_func);

	try {
		this->states.at(state_id);
	}
	catch (const std::out_of_range& oor) {
		this->states[state_id] = s;
		return;
	}

	// should not reach here unless catch is not triggered
	throw "HasState::add_state: state already exists!";
}

HasState::State* HasState::get_state(std::string state_id) {
	return this->states.at(state_id);
}

void HasState::add_transition(
	std::string from_id,
	std::string to_id,
	transition_condition_fptr transition_condition,
	unsigned int transition_probability /* = 1 */
)
{
	// register with HasState outer class
	this->transitions[from_id].push_back(
		HasState::Transition(
			from_id,
			to_id,
			transition_condition,
			transition_probability
		)
	);

	// register with in state and out state
	this->get_state(from_id)->register_transition(this->transitions[from_id].back());
	this->get_state(to_id)->register_transition(this->transitions[from_id].back());
}

void HasState::update(HasState::INPUT input) {
	// take current state and supply input
	// gather list of all out transitions that have transition condition that returns TRUE
	// do weighted random selection and pick next state
	// change current to next and run state function
}