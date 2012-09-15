#include "common.hpp"

#include "StateManager.hpp"

#include "SDLBase.hpp"
#include "InputManager.hpp"
#include "SurfaceManager.hpp"
//#include "StateMovie.hpp"
#include "State.hpp"

using namespace common;

using std::string;
using std::list;

StateManager::StateManager() : fps(0) {
	SDLBase::init();
	InputManager::instance();
	SurfaceManager::instance();
	initStuff();
	loadFirst();
}

StateManager::~StateManager() {
	dumpStates();
	closeStuff();
	SurfaceManager::close();
	InputManager::close();
	SDLBase::close();
}

void StateManager::initStuff() {
	// initializes the root path
	string path = MainArgs::get<string>("-p");
	if (!path.size())
		path = MainArgs::get<string>("--path");
	RootPath::init(path);
	
	// initializes the fps
	if ((MainArgs::find("-f")) || (MainArgs::find("--fps")))
		fps = new Text("", "", 20, 0, SDLBase::getColor(255, 255, 255), Text::shaded, SDLBase::getColor(0, 0, 0));
}

void StateManager::loadFirst() {
	// searches for the game state to be loaded
	string state = MainArgs::get<string>("-s");
	if (!state.size())
		state = MainArgs::get<string>("--state");
	// for a movie as the first game state
	if (state == "StateMovie") {
		// FIXME
		//string movie = MainArgs::get<string>(state);
		//State::states.push_back(new StateMovie(movie));
	}
	// you can't load pause manager as your first game state
	else if (state == "StatePauseManager")
		throw mexception("Trying to load pause manager as the first game state");
	// for specific game states
	else
		State::states.push_back(State::build(state));
}

void StateManager::dumpStates() {
	// to dump this list we must delete the objects
	while (State::states.size()) {
		delete State::states.back();
		State::states.pop_back();
	}
}

void StateManager::closeStuff() {
	// closes the fps text
	if (fps)
		delete fps;
}

void StateManager::run() {
	// the main game loop
	while (true) {
		SDLBase::delayFrame();
		try {
			input();
			update();
			render();
		}
		// push the new game state
		catch(State::StackUp& su) {
			observer::Stack::push();
			State::states.push_back(State::build(su.state(), su.args()));
		}
		// maybe unstack a game state
		catch (State::Unstack&) {
			// throw exception if the list is empty
			if (State::states.size() == 1)
				throw mexception("Trying to drop all game states");
			// unstack a game state
			delete State::states.back();
			State::states.pop_back();
			observer::Stack::pop();
		}
		// change the last pointer
		catch(State::Change& ch) {
			delete State::states.back();
			State::states.back() = State::build(ch.state(), ch.args());
		}
	}
}

void StateManager::input() {
	InputManager::instance()->update();
}

void StateManager::update() {
	// for all loaded states
	for (list<State*>::iterator it = State::states.begin(); it != State::states.end(); ++it) {
		// update if not frozen
		if (!(*it)->frozen())
			(*it)->externUpdate();
	}
}

void StateManager::render() {
	State::states.front()->externRender();
	
	// for all the other loaded states
	list<State*>::iterator it = State::states.begin();
	++it;
	for (it = it; it != State::states.end(); ++it) {
		// render if not frozen
		if (!(*it)->frozen()) {
			SDLBase::renderStackScreen();
			(*it)->externRender();
		}
	}
	
	renderFPS();
	
	SDLBase::updateScreen();
}

void StateManager::renderFPS() {
	if (!fps)
		return;
	
	char tmp[15];
	sprintf(tmp, "FPS: %.1f", SDLBase::FPS());
	
	fps->setText(string(tmp));
	fps->render(SDLBase::screen()->w - fps->w(), 0);
}
