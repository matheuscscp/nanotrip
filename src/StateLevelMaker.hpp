
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
public:
	StateLevelMaker(ArgsBase* args);
	
	void update();
	void render();
};

#endif
