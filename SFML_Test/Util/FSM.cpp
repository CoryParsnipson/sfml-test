#include <stdlib.h>

#include "FSM.h"

// initialize static id variable
unsigned int HasState::Transition::next_id = 0;

HasState::Transition::Transition(std::string from, std::string to, transition_condition_fptr tc, unsigned int prob)
	: id(HasState::Transition::next_id)
	, id_state_from(from)
	, id_state_to(to)
	, prob(prob)
	, tc(tc)
{
	HasState::Transition::next_id++;
}

bool HasState::Transition::will_transition(int input) {
	return this->tc(input);
}

unsigned int HasState::Transition::get_probability() {
	return this->prob;
}

HasState::State::State(HasState* fsm, std::string state_id, std::function<void()> state_func)
	: state_func(state_func)
	, state_id(state_id)
	, fsm(fsm)
{
}

bool HasState::State::has_transition_in(HasState::Transition* t) {
	return this->has_transition(t->id, this->transitions_in);
}

bool HasState::State::has_transition_in(unsigned int id) {
	return this->has_transition(id, this->transitions_in);
}

bool HasState::State::has_transition_out(HasState::Transition* t) {
	return this->has_transition(t->id, this->transitions_out);
}

bool HasState::State::has_transition_out(unsigned int id) {
	return this->has_transition(id, this->transitions_out);
}

bool HasState::State::has_transition(unsigned int id, std::list<HasState::Transition*>& l) {
	std::list<HasState::Transition*>::iterator it;
	for (it = l.begin(); it != l.end(); it++) {
		if ((*it)->id == id) {
			return true;
		}
	}
	return false;
}

void HasState::State::register_transition(HasState::Transition* t) {
	if (this->state_id.compare(t->id_state_from) == 0) {
		// add state_to to list of out states (if not already)
		if (!this->goes_to(t->id_state_to)) {
			this->state_to.push_back(this->fsm->get_state(t->id_state_to));
		}

		// add transition to list of out transitions
		if (!this->has_transition_out(t->id)) {
			this->transitions_out.push_back(t);
		}
	}

	if (this->state_id.compare(t->id_state_to) == 0) {
		// add state_from to list of in states (if not already)
		if (!this->comes_from(t->id_state_from)) {
			this->state_from.push_back(this->fsm->get_state(t->id_state_from));
		}

		// add transition to list of in transitions
		if (!this->has_transition_in(t->id)) {
			this->transitions_in.push_back(t);
		}
	}

	// TODO: warning or exception on no match for both from and to?
}

bool HasState::State::comes_from(std::string state_id) {
	return this->has_state(state_id, this->state_from);
}

bool HasState::State::goes_to(std::string state_id) {
	return this->has_state(state_id, this->state_to);
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

HasState::State* HasState::State::transition(int input) {
	unsigned int total_pr = 0;
	std::vector<unsigned int> list_pr;
	std::vector<HasState::State*> list_state;

	// apply input condition to all outward transitions of this state to gather valid transitions
	std::list<HasState::Transition*>::iterator it;
	for (it = this->transitions_out.begin(); it != this->transitions_out.end(); it++) {
		if (!(*it)->will_transition(input)) {
			continue;
		}

		// transition is valid, add transition information to transition table
		list_pr.push_back((*it)->get_probability());
		list_state.push_back(this->fsm->get_state((*it)->id_state_to));

		total_pr += (*it)->get_probability();
	}

   // if list of out states with valid transitions is empty, we have received invalid input, ignore it
   if (!list_state.size()) {
      return nullptr;
   }

	// normalize probabilities of valid transations to percentages
	std::vector<unsigned int>::iterator norm_it;
	for (norm_it = list_pr.begin(); norm_it != list_pr.end(); norm_it++) {
		*norm_it = (unsigned int)((*norm_it) / (float)(total_pr) * 100);
	}

	// do weighted random probability
	unsigned int idx = 0;
	int magic_num = rand() % 100 + 1;

	while (magic_num > 0 && idx < list_pr.size()) {
		magic_num -= list_pr[idx];
		
		if (magic_num > 0) {
			idx++;
		}
	}

	// return one state from state_to list
	return list_state[idx];
}

HasState::HasState(std::string name)
	: name(name)
	, is_on(false)
{
}

HasState::~HasState() {
	std::map<std::string, HasState::State*>::iterator it;
	for (it = this->states.begin(); it != this->states.end(); it++) {
		delete it->second;
	}
	this->states.clear();

	// delete transition pointers
	std::list<HasState::Transition*>::iterator tr_it;
	for (tr_it = this->transitions.begin(); tr_it != this->transitions.end(); tr_it++) {
		delete *tr_it;
	}
}

void HasState::add_state(std::string state_id, std::function<void()> state_func) {
	HasState::State* s = new HasState::State(this, state_id, state_func);

	try {
		this->states.at(state_id);
	}
	catch (const std::out_of_range& oor) {
		this->states[state_id] = s;

		// if the init state is not defined, set init state to this state
		if (this->init_state_id.empty()) {
			this->init_state_id = state_id;
		}

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
	HasState::Transition* t = new HasState::Transition(
		from_id,
		to_id,
		transition_condition,
		transition_probability
	);

	// register with HasState outer class
	this->transitions.push_back(t);

	// register with in state and out state
	this->get_state(from_id)->register_transition(t);
	this->get_state(to_id)->register_transition(t);
}

void HasState::set_init_state(std::string init_state_id) {
	// will throw exception on invalid state
	this->get_state(init_state_id);
	this->init_state_id = init_state_id;
}

void HasState::on() {
	this->is_on = true;
	this->current_state = this->get_state(this->init_state_id);

	// run state function of on state
	this->current_state->state_func();
}

void HasState::off() {
	this->is_on = false;
	delete this->current_state;
}

void HasState::reset() {
	this->off();

	// reset all state variables... somehow
}

void HasState::update(int input) {
	if (this->init_state_id.empty() || !this->is_on) {
		// TODO: exception or logging?

		std::cout << "(HasState::update) Warning: state machine is off or has no init state." << std::endl;
		return;
	}

	if (this->current_state == nullptr) {
		throw "(HasState::update) Error: current state pointer is invalid.";
	}

	// get next state to transition to
	HasState::State* next_state = this->current_state->transition(input);

   if (next_state == nullptr) {
      // the input we supplied has not resulted in any transition whatsoever
      return;
   }

	// update current state pointer
	this->current_state = next_state;

	// execute next state function
	next_state->state_func();
}

std::string HasState::get_current_state() {
   return this->current_state->state_id;
};
