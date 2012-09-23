
#ifndef STATELEVEL_HPP
#define STATELEVEL_HPP

#include "configfile.hpp"

#include "State.hpp"
#include "Particle.hpp"
#include "Timer.hpp"
#include "Stopwatch.hpp"
#include "Rectangle.hpp"
#include "Audio.hpp"

class StateLevel : public State {
GAMESTATE
public:
	class UnstackArgs : public ArgsBase {
	public:
		enum {
			TRYAGAIN,
			MAINMENU
		};
		
		int op;
		UnstackArgs(int op);
	};
	
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
protected:
	bool is_bg_init;
	bool win_;
	bool lose_;
	Rectangle screen_box;
	
	Sprite* bg;
	Sprite* bg_grad;
	Sprite* bg_nograd;
	Sprite* hud;
	Sprite* eatles;
	
	int life;
	int level_time;
	Timer timer;
	Stopwatch stopwatch;
	
	Sprite* sprite_life;
	Sprite* border_top;
	Sprite* border_right;
	Sprite* border_bottom;
	Sprite* border_left;
	
	Sprite* sprite_avatar;
	Sprite* sprite_hole;
	Sprite* sprite_negative;
	Sprite* sprite_negative_anim;
	Sprite* sprite_neutral;
	Sprite* sprite_positive;
	Sprite* sprite_positive_anim;
	
	Audio* sound_lose;
	
	Text* text_press_space;
	Text* text_time;
	Text* text_you_lose;
	
	Configuration raw;
	
	std::list<Interaction> interactions;
	
	Particle* avatar;
	Particle* hole;
	std::list<Particle*> particles;
	
	Sprite* charge_cursor;
	Sprite* charge_bar;
	lalge::Scalar max_abs_charge;
	lalge::Scalar charge_cursor_position;
public:
	StateLevel(ArgsBase* args);
	~StateLevel();
	
	void handleUnstack(ArgsBase* args);
	
	void update();
	void render();
protected:
	void reload();
	void assemble();
	void assembleAvatar();
	void assembleHole();
	Particle* assembleParticle(const Configuration& conf);
	void clear();
	
	void setTimeText(int seconds);
	
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleMouseMotion(const observer::Event& event, bool& stop);
	void handleTimerDone(const observer::Event& event, bool& stop);
	
	void lose();
	void gameOver();
	void unpinParticles();
};

#endif
