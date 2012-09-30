
#ifndef STATELEVELMAKERQUIT_HPP
#define STATELEVELMAKERQUIT_HPP

#include "State.hpp"
#include "Button.hpp"
#include "Audio.hpp"

class StateLevelMakerQuit : public State {
GAMESTATE
private:
	bool quit;
	
	Sprite* bg;
	
	Button* save;
	Button* discard;
	Button* cancel;
	
	Audio* sound_error;
public:
	StateLevelMakerQuit(ArgsBase* args);
	~StateLevelMakerQuit();
	
	void update();
	void render();
private:
	void handleSave(const observer::Event& event, bool& stop);
	void handleDiscard(const observer::Event& event, bool& stop);
	void handleCancel(const observer::Event& event, bool& stop);
	
	void handleQuit(const observer::Event& event, bool& stop);
	void handleKeyDown(const observer::Event& event, bool& stop);
};

#endif
