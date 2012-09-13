#ifndef STATEMOVIE_HPP
#define STATEMOVIE_HPP

#include "State.hpp"
#include "Sprite.hpp"
#include "Audio.hpp"
#include "Timer.hpp"

class StateMovie : public State {
GAMESTATE
public:
	struct Args {
		std::string path;
		int nextstate;
		bool pause_able;
		bool skip_able;
		int skip_fade;
		
		Args(const std::string& name, int nextstate, bool pause_able = true, bool skip_able = true, int skip_fade = 1500);
	};
private:
	int nextstate;
	bool pause_able;
	bool skip_able;
	int skip_fade;
	
	Sprite* frames;
	int* sizes;
	int amount;
	int index;
	
	Audio* bgm;
	
	Timer timer;
	Timer skip_timer;
public:
	StateMovie(const std::string& movie);
	~StateMovie();
	
	void input();
	void update();
	void render();
private:
	void handleKeyDown(const observer::Event& event, bool& stop);
	
	void loadData(const std::string& path);
};

#endif
