
#ifndef STATEINTRODUCTION_HPP
#define STATEINTRODUCTION_HPP

#include "State.hpp"
#include "Animation.hpp"
#include "Stopwatch.hpp"
#include "Audio.hpp"

class StateIntroduction : public State {
GAMESTATE
private:
	Sprite* storyboard;
	Sprite* truck;
	Sprite* press;
	Sprite* mainmenu;
	Sprite* bg;
	
	Animation* eatles;
	Animation* eatles_states[3];
	
	Stopwatch stopwatch_end;
	Stopwatch stopwatch_eatles;
	Stopwatch stopwatch_fadein;
	
	lalge::R2Vector storyboard_position;
	lalge::R2Vector storyboard_speed;
	lalge::R2Vector storyboard_acceleration;
	
	lalge::R2Vector truck_position;
	lalge::R2Vector truck_speed;
	
	lalge::R2Vector eatles_position;
	lalge::R2Vector eatles_speed;
	
	bool pressed_enter;
	bool played_sound;
	
	float fadein_alpha;
	
	Audio* sound_scream;
	Audio* sound_bird;
	Audio* sound_fall;
	Audio* sound_truck;
	Audio* sound_eatles_getup;
public:
	StateIntroduction(ArgsBase* args);
	~StateIntroduction();
	
	void update();
	void render();
private:
	void updateEatles();
	
	void handleKeyDown(const observer::Event& event, bool& stop);
};

#endif
