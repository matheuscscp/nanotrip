#include <fstream>
#include <sstream>

#include "LevelMakerData.hpp"

#include "common.hpp"

#include "Item.hpp"
#include "Circle.hpp"

using namespace common;
using namespace lalge;

using std::string;
using std::fstream;
using std::list;
using std::stringstream;

const int LevelMakerData::default_level_time = 60;
const Scalar LevelMakerData::default_max_abs_charge = 0.03;
const string LevelMakerData::default_bgm = "";
Sprite* LevelMakerData::sprite_avatar;
Sprite* LevelMakerData::sprite_blackhole;
Sprite* LevelMakerData::sprite_key;
Sprite* LevelMakerData::sprite_negative;
Sprite* LevelMakerData::sprite_neutral;
Sprite* LevelMakerData::sprite_positive;
Sprite* LevelMakerData::sprite_item_time;
Sprite* LevelMakerData::sprite_item_point;
Sprite* LevelMakerData::sprite_item_life;
Sprite* LevelMakerData::sprite_item_mass;
Sprite* LevelMakerData::sprite_item_barrier;
Sprite* LevelMakerData::sprite_item_lethal_barrier;
Sprite* LevelMakerData::sprite_avatar_selection;
Sprite* LevelMakerData::sprite_blackhole_selection;
Sprite* LevelMakerData::sprite_key_selection;
Sprite* LevelMakerData::sprite_particle_selection;
Sprite* LevelMakerData::sprite_item_time_selection;
Sprite* LevelMakerData::sprite_item_point_selection;
Sprite* LevelMakerData::sprite_item_life_selection;
Sprite* LevelMakerData::sprite_item_mass_selection;
Sprite* LevelMakerData::sprite_item_barrier_selection;
Sprite* LevelMakerData::sprite_item_lethal_barrier_selection;

LevelMakerData::LevelMakerData(const string& levelname) : levelname(levelname) {
	load();
}

LevelMakerData::~LevelMakerData() {
	clear();
}

void LevelMakerData::load() {
	fstream f(RootPath::get("level/" + levelname + ".conf").c_str());
	if (!f.is_open()) {
		assembleEmptyLevel();
		return;
	}
	f.close();
	
	assemble();
}

void LevelMakerData::clear() {
	delete avatar;
	delete blackhole;
	if (key)
		delete key;
	
	// all particles
	while (particles.size()) {
		delete particles.back();
		particles.pop_back();
	}
	
	// all items
	while (items.size()) {
		delete items.back();
		items.pop_back();
	}
}

bool LevelMakerData::modified() {
	// if just created the level, must return true
	fstream f(RootPath::get("level/" + levelname + ".conf").c_str());
	if (!f.is_open())
		return true;
	f.close();
	
	// reading the file to make the comparison
	
	LevelMakerData* temp_data = new LevelMakerData(levelname);
	
	// general config
	if (level_time != temp_data->level_time)
		return true;
	if (max_abs_charge != temp_data->max_abs_charge)
		return true;
	if (bgm != temp_data->bgm)
		return true;
	if (has_top != temp_data->has_top)
		return true;
	if (has_right != temp_data->has_right)
		return true;
	if (has_bottom != temp_data->has_bottom)
		return true;
	if (has_left != temp_data->has_left)
		return true;
	
	// particular objects
	if (avatarModified(temp_data->avatar))
		return true;
	if (blackHoleModified(temp_data->blackhole))
		return true;
	if (((!key) && (temp_data->key)) || ((key) && (!temp_data->key)))
		return true;
	if ((key) && (temp_data->key)) {
		if (keyModified(temp_data->key))
			return true;
	}
	
	// particles
	if (particlesModified(temp_data->particles))
		return true;
	
	// items
	if (itemsModified(temp_data->items))
		return true;
	
	delete temp_data;
	return false;
}

void LevelMakerData::save() {
	Configuration level;
	
	// general config
	{
		Configuration general;
		
		general.insertInt("history", 0);
		general.insertInt("level_time", level_time);
		general.insertReal("max_abs_charge", max_abs_charge);
		if (bgm.size())
			general.insertStr("bgm", bgm);
		general.insertInt("border_top", has_top);
		general.insertInt("border_right", has_right);
		general.insertInt("border_bottom", has_bottom);
		general.insertInt("border_left", has_left);
		
		level.insertConfig("general", general);
	}
	
	fetchAvatar(level);
	fetchBlackHole(level);
	fetchKey(level);
	
	// all particles
	int i = 1;
	for (list<LevelMakerObject*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		fetchParticle(i, *it, level);
		++i;
	}
	
	// all items
	i = 1;
	for (list<LevelMakerObject*>::iterator it = items.begin(); it != items.end(); ++it) {
		fetchItem(i, *it, level);
		++i;
	}
	
	level.writeTxt(RootPath::get("level/" + levelname + ".conf"));
}

void LevelMakerData::revert() {
	clear();
	load();
}

void LevelMakerData::deleteSelection() {
	bool avatar_selected = false;
	bool blackhole_selected = false;
	
	while (LevelMakerObject::selected.size()) {
		switch ((*LevelMakerObject::selected.begin())->getType()) {
		case LevelMakerObject::AVATAR:
			LevelMakerObject::selected.erase(avatar);
			avatar_selected = true;
			break;
			
		case LevelMakerObject::BLACKHOLE:
			LevelMakerObject::selected.erase(blackhole);
			blackhole_selected = true;
			break;
			
		case LevelMakerObject::KEY:
			delete key;
			key = 0;
			break;
			
		case LevelMakerObject::PARTICLE:
			particles.remove(*LevelMakerObject::selected.begin());
			delete *LevelMakerObject::selected.begin();
			break;
			
		case LevelMakerObject::ITEM:
			items.remove(*LevelMakerObject::selected.begin());
			delete *LevelMakerObject::selected.begin();
			break;
			
		default:
			break;
		}
	}
	
	if (avatar_selected)
		LevelMakerObject::selected.insert(avatar);
	if (blackhole_selected)
		LevelMakerObject::selected.insert(blackhole);
}

void LevelMakerData::cloneSelection() {
	list<LevelMakerObject*> particles;
	list<LevelMakerObject*> items;
	R2Vector max_diff;
	
	// clone selection
	while (LevelMakerObject::selected.size()) {
		LevelMakerObject* new_object = 0;
		
		switch ((*LevelMakerObject::selected.begin())->getType()) {
		case LevelMakerObject::PARTICLE:
			new_object = (*LevelMakerObject::selected.begin())->clone();
			particles.push_back(new_object);
			break;
			
		case LevelMakerObject::ITEM:
			new_object = (*LevelMakerObject::selected.begin())->clone();
			items.push_back(new_object);
			break;
			
		default:
			break;
		}
		
		// set new position
		if (new_object) {
			R2Vector diff = r2vec(new_object->selection->rectW(), new_object->selection->rectH());
			if (diff.size() > max_diff.size())
				max_diff = diff;
		}
		
		LevelMakerObject::selected.erase(*LevelMakerObject::selected.begin());
	}
	
	// add to my list and select, all new particles
	while (particles.size()) {
		LevelMakerObject* new_object = particles.back();
		
		// position
		new_object->getShape()->position += max_diff;
		new_object->getGameObject()->getShape()->position = new_object->getShape()->position;
		
		LevelMakerObject::selected.insert(new_object);
		this->particles.push_back(new_object);
		particles.pop_back();
	}
	
	// add to my list and select, all new items
	while (items.size()) {
		LevelMakerObject* new_object = items.back();
		
		// position
		new_object->getShape()->position += max_diff;
		new_object->getGameObject()->getShape()->position = new_object->getShape()->position;
		
		LevelMakerObject::selected.insert(new_object);
		this->items.push_back(new_object);
		items.pop_back();
	}
}

void LevelMakerData::assembleEmptyLevel() {
	Particle* avatar_obj = new Particle();
	Particle* blackhole_obj = new Particle();
	Item* key_obj = new Item();
	
	avatar_obj->sprite = sprite_avatar;
	avatar_obj->getShape()->position = r2vec(320, 360);
	avatar_obj->speed = r2vec(50, 0);
	((Circle*)avatar_obj->getShape())->setRadius(avatar_obj->sprite->rectW()/2);
	
	blackhole_obj->sprite = sprite_blackhole;
	blackhole_obj->getShape()->position = r2vec(960, 360);
	((Circle*)blackhole_obj->getShape())->setRadius(blackhole_obj->sprite->rectW()/2);
	
	key_obj->sprite = sprite_key;
	key_obj->getShape()->position = r2vec(640, 360);
	((Circle*)key_obj->getShape())->setRadius(key_obj->sprite->rectW()/2);
	
	// general config
	level_time = default_level_time;
	max_abs_charge = default_max_abs_charge;
	bgm = default_bgm;
	has_top = false;
	has_right = false;
	has_bottom = false;
	has_left = false;
	
	avatar = new LevelMakerObject(LevelMakerObject::AVATAR, avatar_obj);
	blackhole = new LevelMakerObject(LevelMakerObject::BLACKHOLE, blackhole_obj);
	key = new LevelMakerObject(LevelMakerObject::KEY, key_obj);
}

void LevelMakerData::assemble() {
	Configuration level;
	level.readTxt(RootPath::get("level/" + levelname + ".conf"));
	
	assembleAvatar(level.getConfig("avatar"));
	assembleBlackHole(level.getConfig("blackhole"));
	assembleKey(level);
	
	// all particles
	list<Configuration> conf_particles = level.getConfigList("particle");
	for (list<Configuration>::iterator it = conf_particles.begin(); it != conf_particles.end(); ++it)
		assembleParticle(*it);
	
	// all items
	list<Configuration> conf_items = level.getConfigList("item");
	for (list<Configuration>::iterator it = conf_items.begin(); it != conf_items.end(); ++it)
		assembleItem(*it);
	
	// general config
	{
		Configuration general = level.getConfig("general");
		
		level_time = general.getInt("level_time");
		if (level_time > 599)
			level_time = 599;
		else if (level_time < 5)
			level_time = 5;
		
		max_abs_charge = general.getReal("max_abs_charge");
		if (max_abs_charge <= 0)
			max_abs_charge = ((max_abs_charge) ? -max_abs_charge : default_max_abs_charge);
		
		try {
			bgm = general.getStr("bgm");
		}
		catch(Configuration::VarNotFound&) {
			bgm = "";
		}
		
		// borders
		has_top = general.getInt("border_top");
		has_right = general.getInt("border_right");
		has_bottom = general.getInt("border_bottom");
		has_left = general.getInt("border_left");
	}
}

void LevelMakerData::assembleAvatar(const Configuration& conf) {
	Particle* avatar_obj = new Particle();
	avatar_obj->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	avatar_obj->speed = r2vec(conf.getReal("speedX"), conf.getReal("speedY"));
	avatar_obj->setElasticity(conf.getReal("k"));
	avatar_obj->setMass(conf.getReal("m"));
	avatar_obj->setCharge(conf.getReal("q"));
	
	// sprite
	avatar_obj->sprite = sprite_avatar;
	((Circle*)avatar_obj->getShape())->setRadius(avatar_obj->sprite->rectW()/2);
	
	avatar = new LevelMakerObject(LevelMakerObject::AVATAR, avatar_obj);
	avatar->selection = sprite_avatar_selection;
}

void LevelMakerData::assembleBlackHole(const Configuration& conf) {
	Particle* blackhole_obj = new Particle();
	blackhole_obj->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	blackhole_obj->setMass(conf.getReal("m"));
	
	// sprite
	blackhole_obj->sprite = sprite_blackhole;
	((Circle*)blackhole_obj->getShape())->setRadius(blackhole_obj->sprite->rectW()/2);
	
	blackhole = new LevelMakerObject(LevelMakerObject::BLACKHOLE, blackhole_obj);
	blackhole->selection = sprite_blackhole_selection;
}

void LevelMakerData::assembleKey(const Configuration& level) {
	Configuration conf;
	try {
		conf = level.getConfig("key");
	}
	catch(Configuration::VarNotFound&) {
		key = 0;
		return;
	}
	Item* key_obj = new Item();
	key_obj->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	key_obj->setElasticity(conf.getReal("k"));
	key_obj->setMass(conf.getReal("m"));
	
	// sprite
	key_obj->sprite = sprite_key;
	((Circle*)key_obj->getShape())->setRadius(key_obj->sprite->rectW()/2);
	
	key = new LevelMakerObject(LevelMakerObject::KEY, key_obj);
	key->selection = sprite_key_selection;
}

void LevelMakerData::assembleParticle(const Configuration& conf) {
	Particle* particle_obj = new Particle();
	particle_obj->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	particle_obj->setElasticity(conf.getReal("k"));
	particle_obj->setMass(conf.getReal("m"));
	particle_obj->setCharge(conf.getReal("q"));
	
	// sprite
	if (particle_obj->getCharge() == 0)
		particle_obj->sprite = sprite_neutral;
	else if (particle_obj->getCharge() < 0)
		particle_obj->sprite = sprite_negative;
	else
		particle_obj->sprite = sprite_positive;
	((Circle*)particle_obj->getShape())->setRadius(particle_obj->sprite->rectW()/2);
	
	particles.push_back(new LevelMakerObject(LevelMakerObject::PARTICLE, particle_obj));
	particles.back()->selection = sprite_particle_selection;
}

void LevelMakerData::assembleItem(const Configuration& conf) {
	Sprite* selection;
	
	Item* item_obj = new Item();
	item_obj->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	item_obj->operation = conf.getInt("operation");
	item_obj->value = conf.getReal("value");
	if (item_obj->value < 0)
		item_obj->value *= -1;
	item_obj->setElasticity(conf.getReal("k"));
	item_obj->setMass(conf.getReal("m"));
	
	// sprite
	switch (item_obj->operation) {
	case Item::KEY:
		delete item_obj;
		return;
		
	case Item::TIME:
		item_obj->sprite = sprite_item_time;
		selection = sprite_item_time_selection;
		break;
		
	case Item::POINT:
		item_obj->sprite = sprite_item_point;
		selection = sprite_item_point_selection;
		break;
		
	case Item::LIFE:
		item_obj->sprite = sprite_item_life;
		selection = sprite_item_life_selection;
		break;
		
	case Item::MASS:
		item_obj->sprite = sprite_item_mass;
		selection = sprite_item_mass_selection;
		break;
		
	case Item::BARRIER:
		if (!item_obj->value) {
			item_obj->sprite = sprite_item_barrier;
			selection = sprite_item_barrier_selection;
		}
		else {
			item_obj->sprite = sprite_item_lethal_barrier;
			selection = sprite_item_lethal_barrier_selection;
		}
		break;
		
	default:
		break;
	}
	((Circle*)item_obj->getShape())->setRadius(item_obj->sprite->rectW()/2);
	
	items.push_back(new LevelMakerObject(LevelMakerObject::ITEM, item_obj));
	items.back()->selection = selection;
}

void LevelMakerData::fetchAvatar(Configuration& level) const {
	Configuration conf;
	
	conf.insertReal("x", avatar->getGameObject()->getShape()->position.x(0));
	conf.insertReal("y", avatar->getGameObject()->getShape()->position.x(1));
	conf.insertReal("speedX", ((Particle*)avatar->getGameObject())->speed.x(0));
	conf.insertReal("speedY", ((Particle*)avatar->getGameObject())->speed.x(1));
	conf.insertReal("k", ((Particle*)avatar->getGameObject())->getElasticity());
	conf.insertReal("m", ((Particle*)avatar->getGameObject())->getMass());
	conf.insertReal("q", ((Particle*)avatar->getGameObject())->getCharge());
	
	level.insertConfig("avatar", conf);
}

void LevelMakerData::fetchBlackHole(Configuration& level) const {
	Configuration conf;
	
	conf.insertReal("x", blackhole->getGameObject()->getShape()->position.x(0));
	conf.insertReal("y", blackhole->getGameObject()->getShape()->position.x(1));
	conf.insertReal("m", ((Particle*)blackhole->getGameObject())->getMass());
	
	level.insertConfig("blackhole", conf);
}

void LevelMakerData::fetchKey(Configuration& level) const {
	if (!key)
		return;
	
	Configuration conf;
	
	conf.insertReal("x", key->getGameObject()->getShape()->position.x(0));
	conf.insertReal("y", key->getGameObject()->getShape()->position.x(1));
	conf.insertReal("k", ((Particle*)key->getGameObject())->getElasticity());
	conf.insertReal("m", ((Particle*)key->getGameObject())->getMass());
	
	level.insertConfig("key", conf);
}

void LevelMakerData::fetchParticle(int i, LevelMakerObject* particle, Configuration& level) const {
	Configuration conf;
	stringstream ss;
	
	conf.insertReal("x", particle->getGameObject()->getShape()->position.x(0));
	conf.insertReal("y", particle->getGameObject()->getShape()->position.x(1));
	conf.insertReal("k", ((Particle*)particle->getGameObject())->getElasticity());
	conf.insertReal("m", ((Particle*)particle->getGameObject())->getMass());
	conf.insertReal("q", ((Particle*)particle->getGameObject())->getCharge());
	
	ss << "particle";
	ss << i;
	
	level.insertConfig(ss.str(), conf);
}

void LevelMakerData::fetchItem(int i, LevelMakerObject* item, Configuration& level) const {
	Configuration conf;
	stringstream ss;
	
	conf.insertReal("x", item->getGameObject()->getShape()->position.x(0));
	conf.insertReal("y", item->getGameObject()->getShape()->position.x(1));
	conf.insertReal("k", ((Item*)item->getGameObject())->getElasticity());
	conf.insertReal("m", ((Item*)item->getGameObject())->getMass());
	conf.insertReal("value", ((Item*)item->getGameObject())->value);
	conf.insertInt("operation", ((Item*)item->getGameObject())->operation);
	
	ss << "item";
	ss << i;
	
	level.insertConfig(ss.str(), conf);
}

bool LevelMakerData::avatarModified(LevelMakerObject* temp_avatar) {
	Particle* my = (Particle*)avatar->getGameObject();
	Particle* its = (Particle*)temp_avatar->getGameObject();
	
	if (my->getElasticity() != its->getElasticity())
		return true;
	if (my->getMass() != its->getMass())
		return true;
	if (my->getCharge() != its->getCharge())
		return true;
	if (my->getShape()->position != its->getShape()->position)
		return true;
	if (my->speed != its->speed)
		return true;
	
	return false;
}

bool LevelMakerData::blackHoleModified(LevelMakerObject* temp_blackhole) {
	Particle* my = (Particle*)blackhole->getGameObject();
	Particle* its = (Particle*)temp_blackhole->getGameObject();
	
	if (my->getMass() != its->getMass())
		return true;
	if (my->getShape()->position != its->getShape()->position)
		return true;
	
	return false;
}

bool LevelMakerData::keyModified(LevelMakerObject* temp_key) {
	Particle* my = (Particle*)key->getGameObject();
	Particle* its = (Particle*)temp_key->getGameObject();
	
	if (my->getElasticity() != its->getElasticity())
		return true;
	if (my->getMass() != its->getMass())
		return true;
	if (my->getShape()->position != its->getShape()->position)
		return true;
	
	return false;
}

bool LevelMakerData::particlesModified(list<LevelMakerObject*>& temp_particles) {
	if (particles.size() != temp_particles.size())
		return true;
	
	for (list<LevelMakerObject*>::iterator it1 = particles.begin(); it1 != particles.end(); ++it1) {
		// searches my particle in its particles
		list<LevelMakerObject*>::iterator it2 = temp_particles.begin();
		while ((it2 != temp_particles.end()) && (!particlesEquals((*it1)->getGameObject(), (*it2)->getGameObject())))
			++it2;
		
		// if not found, return true
		if (it2 == temp_particles.end())
			return true;
	}
	return false;
}

bool LevelMakerData::itemsModified(list<LevelMakerObject*>& temp_items) {
	if (items.size() != temp_items.size())
		return true;
	
	for (list<LevelMakerObject*>::iterator it1 = items.begin(); it1 != items.end(); ++it1) {
		// searches my particle in its items
		list<LevelMakerObject*>::iterator it2 = temp_items.begin();
		while ((it2 != temp_items.end()) && (!itemsEquals((*it1)->getGameObject(), (*it2)->getGameObject())))
			++it2;
		
		// if not found, return true
		if (it2 == temp_items.end())
			return true;
	}
	return false;
}

bool LevelMakerData::particlesEquals(GameObject* p1, GameObject* p2) {
	Particle* my = (Particle*)p1;
	Particle* its = (Particle*)p2;
	
	if (my->getElasticity() != its->getElasticity())
		return false;
	if (my->getMass() != its->getMass())
		return false;
	if (my->getCharge() != its->getCharge())
		return false;
	if (my->getShape()->position != its->getShape()->position)
		return false;
	
	return true;
}

bool LevelMakerData::itemsEquals(GameObject* i1, GameObject* i2) {
	Item* my = (Item*)i1;
	Item* its = (Item*)i2;
	
	if (my->getElasticity() != its->getElasticity())
		return false;
	if (my->getMass() != its->getMass())
		return false;
	if (my->operation != its->operation)
		return false;
	if (my->value != its->value)
		return false;
	if (my->getShape()->position != its->getShape()->position)
		return false;
	
	return true;
}
