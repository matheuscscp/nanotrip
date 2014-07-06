
#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "common.hpp"

#include "Text.hpp"

/// Class to manage all game states.
class StateManager {
private:
	Text* fps;
public:
	/// Initializes SDL and other stuff.
	StateManager(const std::string& firstState);
	~StateManager();	///< Closes everything.
private:
	void initStuff();
	void loadFirst(const std::string& firstState);
	
	void dumpStates();
	void closeStuff();
public:
	void run();	///< Method to run the game.
private:
	void input();
	void update();
	void render();
	
	void renderFPS();
	
	void handleMexception(common::mexception* e);
};

#endif
