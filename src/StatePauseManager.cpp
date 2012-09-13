#include "StatePauseManager.hpp"

#include "InputManager.hpp"

using std::list;

void StatePauseManager::load(StateArgs* args)
{
	bool connect_ = false;
	
	if (states.size() <= 1)
		connect_ = true;
	else {
		list<State*>::reverse_iterator it = states.rbegin();
		it++;
		
		switch ((*it)->id()) {/*
		case GameStates::MOVIE:
			this->args = new StateArgs( GameStates::MOVIE_PAUSE );
			break;
		*/
		default:
			connect_ = true;
			break;
		}
	}
	
	if( connect_ )
	{
		InputManager::instance()->connect(
			InputManager::QUIT,
			this,
			&StatePauseManager::handleQuit
		);
		InputManager::instance()->connect(
			InputManager::KEYDOWN,
			this,
			&StatePauseManager::handleKeyDown
		);
	}
}

void StatePauseManager::handleKeyDown(const observer::Event& event, bool& stop) {
	int key = inputmanager_event.key.keysym.sym;
	
	if ((key == SDLK_KP_ENTER) || (key == SDLK_RETURN)) {
		args = new StateArgs(GameStates::NULL_OP, GameStates::UNSTACK);
	}
}
