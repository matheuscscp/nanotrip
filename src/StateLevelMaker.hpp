
#ifndef STATELEVELMAKER_HPP
#define STATELEVELMAKER_HPP

#include "configfile.hpp"

#include "State.hpp"
#include "LevelMakerObject.hpp"

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
	
	int bg_x, bg_y;
	Sprite* bg;
	
	int level_time;
	lalge::Scalar max_abs_charge;
	
	Sprite* sprite_avatar;
	Sprite* sprite_blackhole;
	Sprite* sprite_key;
	Sprite* sprite_negative;
	Sprite* sprite_neutral;
	Sprite* sprite_positive;
	Sprite* sprite_item_time;
	Sprite* sprite_item_point;
	Sprite* sprite_item_life;
	Sprite* sprite_item_mass;
	Sprite* sprite_item_barrier;
	Sprite* border_top;
	Sprite* border_right;
	Sprite* border_bottom;
	Sprite* border_left;
	
	bool has_top;
	bool has_right;
	bool has_bottom;
	bool has_left;
	
	LevelMakerObject* avatar;
	LevelMakerObject* blackhole;
	LevelMakerObject* key;
	std::list<LevelMakerObject*> particles;
	std::list<LevelMakerObject*> items;
public:
	StateLevelMaker(ArgsBase* args);
	~StateLevelMaker();
	
	void update();
	void render();
private:
	void load();
	void clear();
	
	void save();
	void reload();
	void test();
	void quit();
	void discard();
	
	void assembleEmptyLevel();
	void assemble();
	void assembleAvatar(const Configuration& conf);
	void assembleBlackHole(const Configuration& conf);
	void assembleKey(const Configuration& level);
	void assembleParticle(const Configuration& conf);
	void assembleItem(const Configuration& conf);
	
	void fetchAvatar(Configuration& level);
	void fetchBlackHole(Configuration& level);
	void fetchKey(Configuration& level);
	void fetchParticle(int i, LevelMakerObject* particle, Configuration& level);
	void fetchItem(int i, LevelMakerObject* item, Configuration& level);
	
	void handleKeyDown(const observer::Event& event, bool& stop);
};

#endif
