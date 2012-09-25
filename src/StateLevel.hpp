
#ifndef STATELEVEL_HPP
#define STATELEVEL_HPP

#include "configfile.hpp"

#include "State.hpp"
#include "Avatar.hpp"
#include "Item.hpp"
#include "Timer.hpp"
#include "Stopwatch.hpp"
#include "Rectangle.hpp"
#include "Audio.hpp"
#include "Animation.hpp"

class StateLevel : public State {
GAMESTATE
public:
	class UnstackArgs : public ArgsBase {
	public:
		enum {
			TRYAGAIN,
			MAINMENU,
			CONTINUE,
			RETRY,
			QUITLEVEL
		};
		
		int op;
		UnstackArgs(int op);
	};
	
	class Args : public ArgsBase {
	public:
		std::string levelname;
		std::string nextstate;
		ArgsBase* nextargs;
		Args(const std::string& levelname, const std::string& nextstate, ArgsBase* nextargs = 0);
	};
	
	class FinalArgs : public ArgsBase {
	public:
		int points;
		ArgsBase* nextargs;
		FinalArgs(int points, ArgsBase* nextargs = 0);
	};
protected:
	bool history;
	std::string nextstate;
	ArgsBase* nextargs;
	
	bool is_bg_init;
	bool lose_;
	bool win_;
	Rectangle screen_box;
	
	Sprite* bg;
	Sprite* bg_grad;
	Sprite* bg_nograd;
	Sprite* hud;
	Sprite* eatles;
	Sprite* eatles_sheets[6];
	Stopwatch eatles_stopwatch;
	
	int points;
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
	Sprite* sprite_key;
	Sprite* sprite_blackhole;
	Sprite* sprite_negative;
	Sprite* sprite_negative_anim;
	Sprite* sprite_neutral;
	Sprite* sprite_positive;
	Sprite* sprite_positive_anim;
	Sprite* sprite_less_time;
	Sprite* sprite_more_time;
	Sprite* sprite_less_point;
	Sprite* sprite_more_point;
	Sprite* sprite_less_life;
	Sprite* sprite_more_life;
	Sprite* sprite_mass;
	
	Audio* sound_lose;
	Audio* sound_win;
	
	Text* text_time;
	Text* text_points;
	Text* text_press_space;
	Text* text_you_lose;
	
	Configuration raw;
	
	std::list<Interaction> interactions;
	Interaction* interaction_blackhole_force;
	Interaction* interaction_blackhole_collision;
	
	Avatar* avatar;
	Item* key;
	Particle* blackhole;
	std::list<Particle*> particles;
	std::list<Item*> items;
	
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
	void assembleKey();
	void assembleBlackHole();
	Particle* assembleParticle(const Configuration& conf);
	Item* assembleItem(const Configuration& conf);
	void clear();
	
	void setTimeText(int seconds);
	
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleMouseMotion(const observer::Event& event, bool& stop);
	void handleTimerDone(const observer::Event& event, bool& stop);
	void handleAvatarBeingSwallowed(const observer::Event& event, bool& stop);
	void handleItemCollision(const observer::Event& event, bool& stop);
	
	void lose();
	void unpinParticles();
	
	void win();
	
	void addPoints(int plus);
};

#endif
