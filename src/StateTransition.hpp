
#ifndef STATETRANSITION_HPP
#define STATETRANSITION_HPP

#include "State.hpp"
#include "Stopwatch.hpp"
#include "Audio.hpp"

class StateTransition : public State {
GAMESTATE
public:
	class Args : public ArgsBase {
	public:
		enum {
			VERY_FIRST = 0,
			FIRST,
			SECOND,
			THIRD,
			LAST
		};
		
		std::string player_name;
		int level;
		int points;
		
		Args(const std::string& player_name, int level = VERY_FIRST);
	};
private:
	Args* args;
	
	Stopwatch stopwatch;
	
	Sprite* bg;
	Sprite* bg1;
	Sprite* bg2;
	
	Audio* sound_open;
	Audio* sound_close;
public:
	StateTransition(ArgsBase* args);
	~StateTransition();
	
	void update();
	void render();
private:
	void saveRanking();
	
	int closeDelay() const;
};

#endif
