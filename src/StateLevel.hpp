
#ifndef STATELEVEL_HPP
#define STATELEVEL_HPP

#include "configfile.hpp"

#include "State.hpp"
#include "Particle.hpp"

class StateLevel : public State {
GAMESTATE
public:
	enum {
		
	};
	
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
protected:
	bool is_bg_init;
	
	Sprite* bg;
	Sprite* hud;
	Sprite* eatles;
	
	int life;
	Sprite* sprite_life;
	
	Sprite* sprite_avatar;
	Sprite* sprite_hole;
	Sprite* sprite_negative;
	Sprite* sprite_neutral;
	Sprite* sprite_positive;
	
	Text* press_space;
	
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
	
	void handleKeyDown(const observer::Event& event, bool& stop);
	void handleMouseMotion(const observer::Event& event, bool& stop);
};

#endif
