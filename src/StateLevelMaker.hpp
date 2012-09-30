
#ifndef STATELEVELMAKER_HPP
#define STATELEVELMAKER_HPP

#include "configfile.hpp"

#include "State.hpp"
#include "LevelMakerObject.hpp"
#include "Button.hpp"

class StateLevelMaker : public State {
GAMESTATE
public:
	class UnstackArgs : public ArgsBase {
	public:
		enum {
			SAVE_MENU,
			SAVE_QUIT,
			MENU,
			QUIT
		};
		
		int op;
		UnstackArgs(int op);
	};
	
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
private:
	std::string levelname;
	
	int bg_x, bg_y;
	Sprite* bg;
	
	Sprite* hud;
	
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
	Sprite* sprite_item_lethal_barrier;
	Sprite* sprite_avatar_selection;
	Sprite* sprite_blackhole_selection;
	Sprite* sprite_key_selection;
	Sprite* sprite_particle_selection;
	Sprite* sprite_item_time_selection;
	Sprite* sprite_item_point_selection;
	Sprite* sprite_item_life_selection;
	Sprite* sprite_item_mass_selection;
	Sprite* sprite_item_barrier_selection;
	Sprite* sprite_item_lethal_barrier_selection;
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
	
	Button* button_save;
	Button* button_revert;
	Button* button_test;
	Button* button_quit;
	Button* button_delete;
	
	bool clicking_button;
public:
	StateLevelMaker(ArgsBase* args);
	~StateLevelMaker();
	
	void handleUnstack(ArgsBase* args);
	
	void update();
	void render();
private:
	void load();
	void clear();
	void save();
	
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
	
	void handleSave(const observer::Event& event, bool& stop);
	void handleRevert(const observer::Event& event, bool& stop);
	void handleTest(const observer::Event& event, bool& stop);
	void handleQuitButton(const observer::Event& event, bool& stop);
	void handleDelete(const observer::Event& event, bool& stop);
	
	void handleKeyDown(const observer::Event& event, bool& stop);
	
	void handleQuit(const observer::Event& event, bool& stop);
	
	void checkSelectionRequests();
	
	void handleMouseLeft(const observer::Event& event, bool& stop);
};

#endif
