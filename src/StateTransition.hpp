
#ifndef STATETRANSITION_HPP
#define STATETRANSITION_HPP

#include "State.hpp"
#include "Stopwatch.hpp"

class StateTransition : public State {
GAMESTATE
public:
	class Args : public ArgsBase {
	public:
		enum {
			VERY_FIRST,
			FIRST,
			SECOND,
			THIRD,
			LAST
		};
		
		std::string player_name;
		char level;
		int points;
		
		Args(const std::string& player_name, char level = VERY_FIRST);
	};
private:
	Args* args;
	
	Stopwatch stopwatch;
	
	Sprite* bg1;
	Sprite* bg2;
public:
	StateTransition(ArgsBase* args);
	~StateTransition();
	
	void update();
	void render();
private:
	void saveRanking();
};

#endif
