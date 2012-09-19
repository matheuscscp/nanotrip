
#ifndef STATELEVELMAKER_HPP
#define STATELEVELMAKER_HPP

#include "State.hpp"

class StateLevelMaker : public State {
GAMESTATE
public:
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
private:
	std::string levelname;
	
	int state;
	
	Sprite* bg;
	
	Sprite* hud;
public:
	StateLevelMaker(ArgsBase* args);
	
	void handleUnstack(ArgsBase* args);
	
	void update();
	void render();
private:
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
};

#endif
