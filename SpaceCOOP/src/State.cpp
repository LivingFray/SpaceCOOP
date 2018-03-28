#include "State.h"

shared_ptr<State> State::currentState;
shared_ptr<State> State::nextState;

State::State() {
}


State::~State() {
}

void State::setState(shared_ptr<State> newState) {
	nextState = newState;
}

shared_ptr<State> State::getState() {
	return currentState;
}

shared_ptr<State> State::advanceState() {
	if (nextState) {
		currentState = nextState;
		nextState->onActivation();
		nextState = NULL;
	}
	return currentState;
}
