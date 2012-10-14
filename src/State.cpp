#include "State.hpp"

#include "InputManager.hpp"

using namespace common;

using std::string;
using std::list;

// =============================================================================
// State::ArgsBase Class
// =============================================================================

State::ArgsBase::ArgsBase() {}
State::ArgsBase::~ArgsBase() {}

// =============================================================================
// State::Args Class
// =============================================================================

State::Change::Change(const string& state, ArgsBase* args) :
mexception("State::Change"), state_(state), args_(args) {}
State::Change::~Change() throw() {}

const string& State::Change::state() const { return state_; }
State::ArgsBase* State::Change::args() const { return args_; }

// =============================================================================
// State::StackUp Class
// =============================================================================

State::StackUp::StackUp(const string& state, ArgsBase* args) :
mexception("State::StackUp"), state_(state), args_(args) {}
State::StackUp::~StackUp() throw() {}

const string& State::StackUp::state() const { return state_; }
State::ArgsBase* State::StackUp::args() const { return args_; }

// =============================================================================
// State::Unstack Class
// =============================================================================

State::Unstack::Unstack(ArgsBase* args) : mexception("State::Unstack"), args_(args) {}
State::Unstack::~Unstack() throw() {}

State::ArgsBase* State::Unstack::args() const { return args_; }

// =============================================================================
// State Class
// =============================================================================

// Static vars
std::map<string, State::Builder> State::builders;
list<State*> State::states;

State::State() : frozen_(false) {
	InputManager::instance()->connect(InputManager::QUIT, this, &State::handleQuit);
}

State::~State() {
	InputManager::instance()->disconnect(this);
}

State* State::build(const string& name, ArgsBase* args) {
	return (*getIdByName(name))(args);
}

State::Builder State::getIdByName(const string& name) {
	// if it is an already loaded game state
	if (builders.find(name) != builders.end())
		return builders[name];
	
	void* handle = SDL_LoadObject(RootPath::get("obj/" + name + ".o").c_str());
	SHOW(SDL_GetError());
	
	// for unknown game states
	if (!handle)
		throw mexception("Trying to load unknown game state");
	
	builders[name] = (Builder)SDL_LoadFunction(handle, string("build" + name).c_str());
	
	SDL_UnloadObject(handle);
	
	return builders[name];
}

void State::handleUnstack(ArgsBase* args) {}

bool State::frozen() const { return frozen_; }

void State::update() {}
void State::render() {}

void State::handleQuit(const observer::Event& event, bool& stop) {
	throw Quit();
}
