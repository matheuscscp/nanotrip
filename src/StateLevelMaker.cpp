#include <fstream>
#include <sstream>

#include "StateLevelMaker.hpp"

#include "InputManager.hpp"
#include "StateLevel.hpp"
#include "Circle.hpp"
#include "LevelMakerPanel.hpp"

using namespace common;
using namespace lalge;

using std::list;
using std::stringstream;

GAMESTATE_DEF(StateLevelMaker)

StateLevelMaker::UnstackArgs::UnstackArgs(int op) : op(op) {}

StateLevelMaker::Args::Args(const std::string& levelname) : levelname(levelname) {}

StateLevelMaker::StateLevelMaker(ArgsBase* args) : clicking_button(false) {
	if (dynamic_cast<Args*>(args))
		levelname = ((Args*)args)->levelname;
	else if (dynamic_cast<StateLevel::FinalArgs*>(args))
		levelname = ((Args*)((StateLevel::FinalArgs*)args)->nextargs)->levelname;
	delete args;
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevelMaker::handleKeyDown);
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &StateLevelMaker::handleMouseLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &StateLevelMaker::handleMouseLeft);
	
	// background
	srand(time(0));
	bg_x = -(rand()%641);
	bg_y = -(rand()%361);
	bg = new Sprite("img/levelmaker/background.png");
	
	// hud
	hud = new Sprite("img/levelmaker/hud.png");
	
	// all sprites
	sprite_avatar = new Sprite("img/levelmaker/avatar.png");
	sprite_blackhole = new Sprite("img/levelmaker/blackhole.png");
	sprite_key = new Sprite("img/levelmaker/item_key.png");
	sprite_negative = new Sprite("img/levelmaker/particle_negative.png");
	sprite_neutral = new Sprite("img/levelmaker/particle_neutral.png");
	sprite_positive = new Sprite("img/levelmaker/particle_positive.png");
	sprite_item_time = new Sprite("img/levelmaker/item_time.png");
	sprite_item_point = new Sprite("img/levelmaker/item_point.png");
	sprite_item_life = new Sprite("img/levelmaker/item_life.png");
	sprite_item_mass = new Sprite("img/levelmaker/item_mass.png");
	sprite_item_barrier = new Sprite("img/levelmaker/item_bounce_star.png");
	sprite_item_lethal_barrier = new Sprite("img/levelmaker/item_death_star.png");
	sprite_avatar_selection = new Sprite("img/levelmaker/avatar_selection.png");
	sprite_blackhole_selection = new Sprite("img/levelmaker/blackhole_selection.png");
	sprite_key_selection = new Sprite("img/levelmaker/item_key_selection.png");
	sprite_particle_selection = new Sprite("img/levelmaker/particle_selection.png");
	sprite_item_time_selection = new Sprite("img/levelmaker/item_time_selection.png");
	sprite_item_point_selection = new Sprite("img/levelmaker/item_point_selection.png");
	sprite_item_life_selection = new Sprite("img/levelmaker/item_life_selection.png");
	sprite_item_mass_selection = new Sprite("img/levelmaker/item_mass_selection.png");
	sprite_item_barrier_selection = new Sprite("img/levelmaker/item_bounce_star_selection.png");
	sprite_item_lethal_barrier_selection = new Sprite("img/levelmaker/item_death_star_selection.png");
	border_top = new Sprite("img/levelmaker/border_top.png");
	border_right = new Sprite("img/levelmaker/border_right.png");
	border_bottom = new Sprite("img/levelmaker/border_bottom.png");
	border_left = new Sprite("img/levelmaker/border_left.png");
	LevelMakerObject::sprite_selection_box = new Sprite("img/levelmaker/selection_box.png");
	LevelMakerObject::sprite_selection_horizontal = new Sprite("img/levelmaker/selection_horizontal.png");
	LevelMakerObject::sprite_selection_vertical = new Sprite("img/levelmaker/selection_vertical.png");
	
	load();
	
	LevelMakerPanel::init();
	
	// all buttons
	
	button_save = new Button(new Sprite("img/levelmaker/button_save.png"));
	button_save->getShape()->position = r2vec(60, 50);
	button_save->connect(Button::CLICKED, this, &StateLevelMaker::handleSave);
	
	button_revert = new Button(new Sprite("img/levelmaker/button_revert.png"));
	button_revert->getShape()->position = r2vec(160, 50);
	button_revert->connect(Button::CLICKED, this, &StateLevelMaker::handleRevert);
	
	button_test = new Button(new Sprite("img/levelmaker/button_test.png"));
	button_test->getShape()->position = r2vec(60, 100);
	button_test->connect(Button::CLICKED, this, &StateLevelMaker::handleTest);
	
	button_quit = new Button(new Sprite("img/levelmaker/button_quit.png"));
	button_quit->getShape()->position = r2vec(160, 100);
	button_quit->connect(Button::CLICKED, this, &StateLevelMaker::handleQuitButton);
	
	button_delete = new Button(new Sprite("img/levelmaker/button_delete.png"));
	button_delete->getShape()->position = r2vec(60, 150);
	button_delete->connect(Button::CLICKED, this, &StateLevelMaker::handleDelete);
}

StateLevelMaker::~StateLevelMaker() {
	delete bg;
	delete hud;
	
	// all sprites
	delete sprite_avatar;
	delete sprite_blackhole;
	delete sprite_key;
	delete sprite_negative;
	delete sprite_neutral;
	delete sprite_positive;
	delete sprite_item_time;
	delete sprite_item_point;
	delete sprite_item_life;
	delete sprite_item_mass;
	delete sprite_item_barrier;
	delete sprite_item_lethal_barrier;
	delete sprite_avatar_selection;
	delete sprite_blackhole_selection;
	delete sprite_key_selection;
	delete sprite_particle_selection;
	delete sprite_item_time_selection;
	delete sprite_item_point_selection;
	delete sprite_item_life_selection;
	delete sprite_item_mass_selection;
	delete sprite_item_barrier_selection;
	delete sprite_item_lethal_barrier_selection;
	delete border_top;
	delete border_right;
	delete border_bottom;
	delete border_left;
	delete LevelMakerObject::sprite_selection_box;
	LevelMakerObject::sprite_selection_box = 0;
	delete LevelMakerObject::sprite_selection_horizontal;
	LevelMakerObject::sprite_selection_horizontal = 0;
	delete LevelMakerObject::sprite_selection_vertical;
	LevelMakerObject::sprite_selection_vertical = 0;
	
	clear();
	
	LevelMakerPanel::close();
	
	// all buttons
	
	delete button_save->sprite;
	delete button_save;
	
	delete button_revert->sprite;
	delete button_revert;
	
	delete button_test->sprite;
	delete button_test;
	
	delete button_quit->sprite;
	delete button_quit;
	
	delete button_delete->sprite;
	delete button_delete;
}

void StateLevelMaker::handleUnstack(ArgsBase* args) {
	frozen_ = false;
	
	if (!args)
		return;
	
	int op = ((UnstackArgs*)args)->op;
	delete args;
	
	switch (op) {
	case UnstackArgs::SAVE_MENU:
		save();
		throw new Change("StateMakeLevel");
		
	case UnstackArgs::SAVE_QUIT:
		save();
		throw Quit();
		
	case UnstackArgs::MENU:
		throw new Change("StateMakeLevel");
		
	case UnstackArgs::QUIT:
		throw Quit();
		
	default:
		break;
	}
}

void StateLevelMaker::update() {
	checkSelectionRequests();
	
	LevelMakerPanel::checkSelectionRequests();
	
	LevelMakerObject::updateSelection();
	LevelMakerObject::updateSelected();
	
	LevelMakerPanel::updateCurrent();
	
	button_save->update();
	button_revert->update();
	button_test->update();
	button_quit->update();
	button_delete->update();
}

void StateLevelMaker::render() {
	bg->render(bg_x, bg_y);
	
	blackhole->render();
	
	// all particles
	for (list<LevelMakerObject*>::iterator it = particles.begin(); it != particles.end(); ++it)
		(*it)->render();
	
	// all items
	if (key)
		key->render();
	for (list<LevelMakerObject*>::iterator it = items.begin(); it != items.end(); ++it)
		(*it)->render();
	
	avatar->render();
	
	LevelMakerObject::renderSelection();
	
	// borders
	if (has_top)
		border_top->render(30, 0);
	if (has_right)
		border_right->render(1250, 0);
	if (has_bottom)
		border_bottom->render(0, 666);
	if (has_left)
		border_left->render();
	
	// hud
	hud->render();
	button_save->render();
	button_revert->render();
	button_test->render();
	button_quit->render();
	button_delete->render();
	
	// panel
	LevelMakerPanel::renderCurrent();
}

void StateLevelMaker::load() {
	std::fstream f(RootPath::get("level/" + levelname + ".conf").c_str());
	
	if (!f.is_open()) {
		assembleEmptyLevel();
		return;
	}
	
	f.close();
	
	assemble();
}

void StateLevelMaker::clear() {
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

void StateLevelMaker::save() {
	Configuration level;
	
	// general config
	{
		Configuration general;
		
		general.insertInt("history", 0);
		general.insertInt("level_time", level_time);
		general.insertReal("max_abs_charge", max_abs_charge);
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

void StateLevelMaker::assembleEmptyLevel() {
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
	level_time = 60;
	max_abs_charge = 0.03;
	has_top = false;
	has_right = false;
	has_bottom = false;
	has_left = false;
	
	avatar = new LevelMakerObject(LevelMakerObject::AVATAR, avatar_obj);
	blackhole = new LevelMakerObject(LevelMakerObject::BLACKHOLE, blackhole_obj);
	key = new LevelMakerObject(LevelMakerObject::KEY, key_obj);
}

void StateLevelMaker::assemble() {
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
		
		// borders
		level_time = general.getInt("level_time");
		max_abs_charge = general.getReal("max_abs_charge");
		has_top = general.getInt("border_top");
		has_right = general.getInt("border_right");
		has_bottom = general.getInt("border_bottom");
		has_left = general.getInt("border_left");
	}
}

void StateLevelMaker::assembleAvatar(const Configuration& conf) {
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

void StateLevelMaker::assembleBlackHole(const Configuration& conf) {
	Particle* blackhole_obj = new Particle();
	blackhole_obj->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	blackhole_obj->setMass(conf.getReal("m"));
	
	// sprite
	blackhole_obj->sprite = sprite_blackhole;
	((Circle*)blackhole_obj->getShape())->setRadius(blackhole_obj->sprite->rectW()/2);
	
	blackhole = new LevelMakerObject(LevelMakerObject::BLACKHOLE, blackhole_obj);
	blackhole->selection = sprite_blackhole_selection;
}

void StateLevelMaker::assembleKey(const Configuration& level) {
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

void StateLevelMaker::assembleParticle(const Configuration& conf) {
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

void StateLevelMaker::assembleItem(const Configuration& conf) {
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

void StateLevelMaker::fetchAvatar(Configuration& level) {
	Configuration conf;
	
	conf.insertReal("x", avatar->getObject()->getShape()->position.x(0));
	conf.insertReal("y", avatar->getObject()->getShape()->position.x(1));
	conf.insertReal("speedX", ((Particle*)avatar->getObject())->speed.x(0));
	conf.insertReal("speedY", ((Particle*)avatar->getObject())->speed.x(1));
	conf.insertReal("k", ((Particle*)avatar->getObject())->getElasticity());
	conf.insertReal("m", ((Particle*)avatar->getObject())->getMass());
	conf.insertReal("q", ((Particle*)avatar->getObject())->getCharge());
	
	level.insertConfig("avatar", conf);
}

void StateLevelMaker::fetchBlackHole(Configuration& level) {
	Configuration conf;
	
	conf.insertReal("x", blackhole->getObject()->getShape()->position.x(0));
	conf.insertReal("y", blackhole->getObject()->getShape()->position.x(1));
	conf.insertReal("m", ((Particle*)blackhole->getObject())->getMass());
	
	level.insertConfig("blackhole", conf);
}

void StateLevelMaker::fetchKey(Configuration& level) {
	if (!key)
		return;
	
	Configuration conf;
	
	conf.insertReal("x", key->getObject()->getShape()->position.x(0));
	conf.insertReal("y", key->getObject()->getShape()->position.x(1));
	conf.insertReal("k", ((Particle*)key->getObject())->getElasticity());
	conf.insertReal("m", ((Particle*)key->getObject())->getMass());
	
	level.insertConfig("key", conf);
}

void StateLevelMaker::fetchParticle(int i, LevelMakerObject* particle, Configuration& level) {
	Configuration conf;
	stringstream ss;
	
	conf.insertReal("x", particle->getObject()->getShape()->position.x(0));
	conf.insertReal("y", particle->getObject()->getShape()->position.x(1));
	conf.insertReal("k", ((Particle*)particle->getObject())->getElasticity());
	conf.insertReal("m", ((Particle*)particle->getObject())->getMass());
	conf.insertReal("q", ((Particle*)particle->getObject())->getCharge());
	
	ss << "particle";
	ss << i;
	
	level.insertConfig(ss.str(), conf);
}

void StateLevelMaker::fetchItem(int i, LevelMakerObject* item, Configuration& level) {
	Configuration conf;
	stringstream ss;
	
	conf.insertReal("x", item->getObject()->getShape()->position.x(0));
	conf.insertReal("y", item->getObject()->getShape()->position.x(1));
	conf.insertReal("k", ((Item*)item->getObject())->getElasticity());
	conf.insertReal("m", ((Item*)item->getObject())->getMass());
	conf.insertReal("value", ((Item*)item->getObject())->value);
	conf.insertInt("operation", ((Item*)item->getObject())->operation);
	
	ss << "item";
	ss << i;
	
	level.insertConfig(ss.str(), conf);
}

void StateLevelMaker::handleSave(const observer::Event& event, bool& stop) {
	save();
}

void StateLevelMaker::handleRevert(const observer::Event& event, bool& stop) {
	clear();
	load();
}

void StateLevelMaker::handleTest(const observer::Event& event, bool& stop) {
	save();
	throw new Change("StateLevel", new StateLevel::Args(levelname, "StateLevelMaker", new Args(levelname)));
}

void StateLevelMaker::handleQuitButton(const observer::Event& event, bool& stop) {
	frozen_ = true;
	throw new StackUp("StateLevelMakerQuit");
}

void StateLevelMaker::handleDelete(const observer::Event& event, bool& stop) {
	bool avatar_selected = false;
	bool blackhole_selected = false;
	
	while (LevelMakerObject::selected.begin() != LevelMakerObject::selected.end()) {
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

void StateLevelMaker::handleKeyDown(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_DELETE)
		handleDelete(event, stop);
}

void StateLevelMaker::handleQuit(const observer::Event& event, bool& stop) {
	frozen_ = true;
	throw new StackUp("StateLevelMakerQuit", new ArgsBase());
}

void StateLevelMaker::checkSelectionRequests() {
	if (clicking_button) {
		LevelMakerObject::deselection_requested = false;
		LevelMakerObject::selection_requested = false;
	}
}

void StateLevelMaker::handleMouseLeft(const observer::Event& event, bool& stop) {
	clicking_button = false;
	if ((button_save->getShape()->mouseDownInside()) ||
		(button_revert->getShape()->mouseDownInside()) ||
		(button_test->getShape()->mouseDownInside()) ||
		(button_quit->getShape()->mouseDownInside()) ||
		(button_delete->getShape()->mouseDownInside())) {
		clicking_button = true;
	}
}
