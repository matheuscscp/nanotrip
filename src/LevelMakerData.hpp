
#ifndef LEVELMAKERDATA_HPP
#define LEVELMAKERDATA_HPP

#include "configfile.hpp"

#include "LevelMakerObject.hpp"

class LevelMakerData {
public:
	static Sprite* sprite_avatar;
	static Sprite* sprite_blackhole;
	static Sprite* sprite_key;
	static Sprite* sprite_negative;
	static Sprite* sprite_neutral;
	static Sprite* sprite_positive;
	static Sprite* sprite_item_time;
	static Sprite* sprite_item_point;
	static Sprite* sprite_item_life;
	static Sprite* sprite_item_mass;
	static Sprite* sprite_item_barrier;
	static Sprite* sprite_item_lethal_barrier;
	static Sprite* sprite_avatar_selection;
	static Sprite* sprite_blackhole_selection;
	static Sprite* sprite_key_selection;
	static Sprite* sprite_particle_selection;
	static Sprite* sprite_item_time_selection;
	static Sprite* sprite_item_point_selection;
	static Sprite* sprite_item_life_selection;
	static Sprite* sprite_item_mass_selection;
	static Sprite* sprite_item_barrier_selection;
	static Sprite* sprite_item_lethal_barrier_selection;
	
	std::string levelname;
	
	int level_time;
	lalge::Scalar max_abs_charge;
	std::string bgm;
	bool has_top;
	bool has_right;
	bool has_bottom;
	bool has_left;
	
	LevelMakerObject* avatar;
	LevelMakerObject* blackhole;
	LevelMakerObject* key;
	std::list<LevelMakerObject*> particles;
	std::list<LevelMakerObject*> items;
	
	LevelMakerData(const std::string& levelname);
	~LevelMakerData();
private:
	void load();
	void clear();
public:
	bool modified();
	
	void save();
	void revert();
	void deleteSelection();
	void cloneSelection();
private:
	void assembleEmptyLevel();
	void assemble();
	void assembleAvatar(const Configuration& conf);
	void assembleBlackHole(const Configuration& conf);
	void assembleKey(const Configuration& level);
	void assembleParticle(const Configuration& conf);
	void assembleItem(const Configuration& conf);
	
	void fetchAvatar(Configuration& level) const;
	void fetchBlackHole(Configuration& level) const;
	void fetchKey(Configuration& level) const;
	void fetchParticle(int i, LevelMakerObject* particle, Configuration& level) const;
	void fetchItem(int i, LevelMakerObject* item, Configuration& level) const;
	
	bool avatarModified(LevelMakerObject* temp_avatar);
	bool blackHoleModified(LevelMakerObject* temp_blackhole);
	bool keyModified(LevelMakerObject* temp_key);
	bool particlesModified(std::list<LevelMakerObject*>& temp_particles);
	bool itemsModified(std::list<LevelMakerObject*>& temp_items);
	bool particlesEquals(GameObject* p1, GameObject* p2);
	bool itemsEquals(GameObject* i1, GameObject* i2);
};

#endif
