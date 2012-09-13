#include "State.hpp"

#include "common.hpp"

#include "InputManager.hpp"

using namespace common;

using std::string;

// =============================================================================
// State::ArgsBase Class
// =============================================================================

State::ArgsBase::ArgsBase() {}
State::ArgsBase::~ArgsBase() {}

// =============================================================================
// State::Args Class
// =============================================================================

State::Change::Change(const string& state, ArgsBase* args) : state_(state), args_(args) {}
const string& State::Change::state() const { return state_; }
State::ArgsBase* State::Change::args() const { return args_; }

// =============================================================================
// State::StackUp Class
// =============================================================================

State::StackUp::StackUp(const std::string& state, ArgsBase* args) : Change(state, args) {}

// =============================================================================
// State Class
// =============================================================================

// Static vars
std::map<string, State* (*)(State::ArgsBase* args)> State::builders;
std::list<State*> State::states;

State::State() : frozen_(false) {
	InputManager::instance()->connect(InputManager::QUIT, this, &State::handleQuit);
}

State::~State() {
	InputManager::instance()->disconnect(this);
}

State* State::build(const string& name, ArgsBase* args) {
	// for unknown game states
	if (builders.find(name) == builders.end())
		throw mexception("Trying to load unknown game state");
	return (*builders[name])(args);
}

bool State::frozen() const { return frozen_; }

void State::render() {}
void State::input() {}
void State::update() {}

void State::handleQuit(const observer::Event& event, bool& stop) {
	throw Quit();
}
