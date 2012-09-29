#include <sstream>

#include "StateLevel.hpp"

#include "Circle.hpp"
#include "InputManager.hpp"
#include "SDLBase.hpp"

#define GRADIENT_CONST	15000

#define SOUND_DELAY		3
#define EATLES_DELAY	4

#define TIME_POINTS		1
#define LIFE_POINTS		1

#define MAX_CURSOR_X	587

using namespace common;
using namespace lalge;

using std::string;
using std::list;
using std::stringstream;

GAMESTATE_DEF(StateLevel)

StateLevel::UnstackArgs::UnstackArgs(int op) : op(op) {}

StateLevel::Args::Args(const string& levelname, const string& nextstate, ArgsBase* nextargs) :
levelname(levelname), nextstate(nextstate), nextargs(nextargs) {}

StateLevel::FinalArgs::FinalArgs(int points, ArgsBase* nextargs) :
points(points), nextargs(nextargs) {}

StateLevel::StateLevel(ArgsBase* args) :
nextstate(((Args*)args)->nextstate),
nextargs(((Args*)args)->nextargs),
is_bg_init(false),
lose_(false),
win_(false),
life(3),
border_top(0),
border_right(0),
border_bottom(0),
border_left(0),
interaction_blackhole_force(0),
interaction_blackhole_collision(0),
max_abs_charge(1),
charge_cursor_position(640)
{
	SDL_ShowCursor(0);
	
	// screen box
	screen_box.position = r2vec(640, 360);
	screen_box.setWidth(1280);
	screen_box.setHeight(720);
	
	// background
	srand(time(0));
	bg_x = -(rand()%641);
	bg_y = -(rand()%361);
	bg_grad = new Sprite("img/level/background.png");
	bg_nograd = new Sprite("img/level/background.png");
	bg_grad->render();
	bg_grad->setAlpha(0.3f);
	bg_nograd->setAlpha(0.15f);
	
	// hud
	hud = new Sprite("img/level/hud.png");
	eatles_sheets[0] = new Sprite("img/level/eatles.png");
	eatles_sheets[1] = new Animation("img/level/eatles_blink.png", 0, 16, 1, 4);
	eatles_sheets[2] = new Animation("img/level/eatles_wait.png", 0, 20, 1, 18);
	eatles_sheets[3] = new Animation("img/level/eatles_pissed.png", 0, 10, 1, 4);
	eatles_sheets[4] = new Animation("img/level/eatles_laugh.png", 0, 8, 1, 4);
	sprite_life = new Animation("img/level/life.png", 3, 1, 4, 1);
	
	// all sprites
	sprite_avatar = new Animation("img/level/avatar_positive.png", 0, 7, 1, 16);
	sprite_key = new Animation("img/level/item_key.png", 0, 8, 1, 4);
	sprite_blackhole = new Animation("img/level/blackhole.png", 0, 30, 1, 20);
	sprite_negative = new Sprite("img/level/particle_negative.png");
	sprite_negative_anim = new Animation("img/level/particle_negative_ssheet.png", 0, 20, 1, 9);
	sprite_neutral = new Sprite("img/level/particle_neutral.png");
	sprite_positive = new Sprite("img/level/particle_positive.png");
	sprite_positive_anim = new Animation("img/level/particle_positive_ssheet.png", 0, 20, 1, 9);
	sprite_item_time = new Sprite("img/level/item_time.png");
	sprite_item_point = new Sprite("img/level/item_point.png");
	sprite_item_life = new Sprite("img/level/item_life.png");
	sprite_item_mass = new Sprite("img/level/item_mass.png");
	sprite_item_barrier = new Sprite("img/level/item_bounce_star.png");
	sprite_item_lethal_barrier = new Sprite("img/level/item_death_star.png");
	
	// all sounds
	sound_lose = new Audio("sfx/level/lose.wav");
	sound_win = new Audio("sfx/level/win.wav");
	Particle::sound_collision_elastic = new Audio("sfx/level/collision_elastic.wav");
	Particle::sound_collision_inelastic = new Audio("sfx/level/collision_inelastic.wav");
	Audio::musicVolume(0.5);
	
	// configuration file
	raw.readTxt(RootPath::get("level/" + ((Args*)args)->levelname + ".conf"));
	delete args;
	
	// input hooks
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevel::handleKeyDown);
	InputManager::instance()->connect(InputManager::MOUSEMOTION, this, &StateLevel::handleMouseMotion);
	
	// texts
	text_time = new Text("ttf/Swiss721BlackRoundedBT.ttf", "0:00", 13, 0, SDLBase::getColor(255, 31, 77), Text::blended);
	text_points = new Text("ttf/Swiss721BlackRoundedBT.ttf", "0", 12, 0, SDLBase::getColor(65, 217, 255), Text::blended);
	text_press_space = new Text("ttf/Swiss721BlackRoundedBT.ttf", "Press space", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	
	// charge changer
	charge_bar = new Sprite("img/level/charge_bar.png");
	charge_cursor = new Sprite("img/level/charge_cursor.png");
	
	// general config
	{
		Configuration general = raw.getConfig("general");
		
		history = general.getInt("history");
		
		// level time
		level_time = general.getInt("level_time");
		if (level_time < 5)
			level_time = 5;
		else if (level_time > 599)
			level_time = 599;
		timer.connect(Timer::DONE, this, &StateLevel::handleTimerDone);
		
		max_abs_charge = general.getReal("max_abs_charge");
		
		// borders
		if (general.getInt("border_top"))
			border_top = new Sprite("img/level/border_top.png");
		if (general.getInt("border_right"))
			border_right = new Sprite("img/level/border_right.png");
		if (general.getInt("border_bottom"))
			border_bottom = new Sprite("img/level/border_bottom.png");
		if (general.getInt("border_left"))
			border_left = new Sprite("img/level/border_left.png");
		
		loadBGM(general);
	}
	
	assemble();
}

StateLevel::~StateLevel() {
	SDL_ShowCursor(1);
	
	clear();
	
	// all sprites
	delete bg_grad;
	delete bg_nograd;
	delete hud;
	for (int i = 0; i < 5; ++i)
		delete eatles_sheets[i];
	delete sprite_life;
	delete sprite_avatar;
	delete sprite_key;
	delete sprite_blackhole;
	delete sprite_negative;
	delete sprite_negative_anim;
	delete sprite_neutral;
	delete sprite_positive;
	delete sprite_positive_anim;
	delete sprite_item_time;
	delete sprite_item_point;
	delete sprite_item_life;
	delete sprite_item_mass;
	delete sprite_item_barrier;
	delete sprite_item_lethal_barrier;
	
	// all sounds
	delete sound_lose;
	delete sound_win;
	delete Particle::sound_collision_elastic;
	delete Particle::sound_collision_inelastic;
	delete bgm;
	
	// borders
	if (border_top)
		delete border_top;
	if (border_right)
		delete border_right;
	if (border_bottom)
		delete border_bottom;
	if (border_left)
		delete border_left;
	
	// texts
	delete text_time;
	delete text_points;
	delete text_press_space;
	
	// charge cursor
	delete charge_bar;
	delete charge_cursor;
}

void StateLevel::handleUnstack(ArgsBase* args) {
	SDL_ShowCursor(0);
	frozen_ = false;
	if (!args)
		return;
	
	int op = ((UnstackArgs*)args)->op;
	delete args;
	
	switch (op) {
	case UnstackArgs::RETRY:
		lose();
		break;
		
	case UnstackArgs::TRYAGAIN:
		life = 3;
		((Animation*)sprite_life)->setFrame(life);
		reload();
		break;
		
	case UnstackArgs::MENU:
		// next if not nanotrip history
		if (!history)
			throw new Change(nextstate, new FinalArgs(points, nextargs));
		
		if (nextargs)
			delete nextargs;
		
		if (life < 0)
			throw new Change("StateInstructions");
		
		throw new Change("StatePlay");
		break;
		
	case UnstackArgs::NEXT:
		throw new Change(nextstate, new FinalArgs(points, nextargs));
		break;
		
	default:
		break;
	}
}

void StateLevel::update() {
	// timer
	timer.update();
	
	// animations
	sprite_avatar->update();
	sprite_blackhole->update();
	sprite_key->update();
	if ((SDL_GetTicks()/5) % 2) {
		((Animation*)sprite_negative_anim)->setFrame(rand()%9);
		((Animation*)sprite_positive_anim)->setFrame(rand()%9);
	}
	
	// all interactions
	for (list<Interaction>::iterator it = interactions.begin(); it != interactions.end(); ++it) {
		it->interact();
	}
	
	// the avatar
	avatar->update();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->update();
	}
	
	// all items
	for (list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
		(*it)->update();
	}
	
	// eatles
	if (eatles)
		eatles->update();
	
	if (states.back()->id() != StateLevel::getid()) {}	// avoiding throws while stacked
	else if ((!lose_) && (!win_)) {
		int fps = SDLBase::FPS();
		// eatles go back to normal sprite
		if (eatles != eatles_sheets[0]) {
			if (((Animation*)eatles)->getTimeSize() <= eatles_stopwatch.time())
				eatles = eatles_sheets[0];
		}
		// eatles get an animation
		else if ((fps) && (!(rand()%(fps*EATLES_DELAY)))) {
			eatles = eatles_sheets[rand()%2 + 1];
			((Animation*)eatles)->setFrame(0);
			eatles_stopwatch.start();
		}
		
		// update the time text only after the level starts
		if (!avatar->pinned)
			setTimeText(round(float(timer.time())/1000));
		
		// checking if the avatar is far away
		if (!screen_box.Shape::pointInside(avatar->getShape()->position))
			lose();
	}
	else if (stopwatch.time() >= SOUND_DELAY*1000) {
		if (lose_) {
			// if it was the last try
			if (life < 0) {
				eatles = 0;
				SDL_ShowCursor(1);
				throw new StackUp("StateYouLose");
			}
			reload();
		}
		else {
			eatles = 0;
			SDL_ShowCursor(1);
			throw new StackUp("StateYouWin", new FinalArgs(points));
		}
	}
}

void StateLevel::render() {
	// background
	bg->render(bg_x, bg_y);
	
	// the blackhole
	blackhole->render();
	
	// all particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->render();
	}
	
	// all items
	for (list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
		(*it)->render();
	}
	
	// the avatar
	avatar->render();
	
	// borders
	if (border_top)
		border_top->render(30, 0);
	if (border_right)
		border_right->render(1250, 0);
	if (border_bottom)
		border_bottom->render(0, 666);
	if (border_left)
		border_left->render();
	
	// hud
	hud->render();
	if ((eatles) && (!frozen_))
		eatles->render(37, 13);
	text_time->render(289, 53);
	text_points->render(281, 115);
	sprite_life->render(236, 161);
	
	// charge changer
	charge_bar->render(0, 666);
	charge_cursor->render(charge_cursor_position, 709, true);
	
	// press space to start
	if ((avatar->pinned) && ((SDL_GetTicks()/600) % 2) && (!frozen_))
		text_press_space->render(640, 355);
}

void StateLevel::loadBGM(const Configuration& general) {
	stringstream ss;
	
	ss << "sfx/level/";
	try {
		ss << general.getStr("bgm");
	}
	catch (Configuration::VarNotFound&) {
		ss << "song";
		ss << rand()%3;
	}
	ss << ".mp3";
	
	bgm = new Audio(ss.str());
	bgm->play();
}

void StateLevel::reload() {
	clear();
	lose_ = false;
	win_ = false;
	assemble();
}

void StateLevel::assemble() {
	eatles = eatles_sheets[0];
	points = 0;
	addPoints(0);
	setTimeText(level_time);
	
	bg = bg_grad;
	
	assembleAvatar();
	
	assembleBlackHole();
	
	// avatar-blackhole interactions
	interactions.push_back(Interaction(avatar, blackhole, (Interaction::callback)&Particle::addParticleFieldForces, true));
	interaction_blackhole_force = &interactions.back();
	interactions.push_back(Interaction(avatar, blackhole, (Interaction::callback)&Avatar::checkBlackHoleCollision));
	interaction_blackhole_collision = &interactions.back();
	
	Item* key = assembleKey();
	
	if (key) {
		// disable blackhole
		blackhole->hidden = true;
		interaction_blackhole_force->enabled = false;
		interaction_blackhole_collision->enabled = false;
		
		// avatar-key interaction
		interactions.push_back(Interaction(key, avatar, (Interaction::callback)&Item::checkAvatarCollision));
		interactions_avatar_item.push_back(&interactions.back());
		
		items.push_back(key);
	}
	
	// all particles
	list<Configuration> conf_particles = raw.getConfigList("particle");
	for (list<Configuration>::iterator it1 = conf_particles.begin(); it1 != conf_particles.end(); ++it1) {
		Particle* particle = assembleParticle(*it1);
		
		// avatar interactions
		interactions.push_back(Interaction(particle, avatar, (Interaction::callback)&Particle::manageParticleCollision));
		interactions.push_back(Interaction(particle, avatar, (Interaction::callback)&Particle::addParticleFieldForces, true));
		
		// key interactions
		if (key) {
			interactions.push_back(Interaction(particle, key, (Interaction::callback)&Particle::manageParticleCollision));
			interactions.push_back(Interaction(particle, key, (Interaction::callback)&Particle::addParticleFieldForces, true));
		}
		
		// mutual interactions
		for (list<Particle*>::iterator it2 = particles.begin(); it2 != particles.end(); ++it2) {
			interactions.push_back(Interaction(particle, *it2, (Interaction::callback)&Particle::manageParticleCollision));
			interactions.push_back(Interaction(particle, *it2, (Interaction::callback)&Particle::addParticleFieldForces, true));
		}
		
		particles.push_back(particle);
	}
	is_bg_init = true;
	
	// all items
	list<Configuration> conf_items = raw.getConfigList("item");
	for (list<Configuration>::iterator it1 = conf_items.begin(); it1 != conf_items.end(); ++it1) {
		Item* item = assembleItem(*it1);
		
		// avatar interactions
		interactions.push_back(Interaction(item, avatar, (Interaction::callback)&Item::checkAvatarCollision));
		interactions_avatar_item.push_back(&interactions.back());
		
		// mutual interactions
		for (list<Item*>::iterator it2 = items.begin(); it2 != items.end(); ++it2) {
			interactions.push_back(Interaction(item, *it2, (Interaction::callback)&Particle::manageParticleCollision));
			interactions.push_back(Interaction(item, *it2, (Interaction::callback)&Particle::addParticleFieldForces, true));
		}
		
		// particles interactions
		for (list<Particle*>::iterator it2 = particles.begin(); it2 != particles.end(); ++it2) {
			interactions.push_back(Interaction(item, *it2, (Interaction::callback)&Particle::manageParticleCollision));
			interactions.push_back(Interaction(item, *it2, (Interaction::callback)&Particle::addParticleFieldForces, true));
		}
		
		items.push_back(item);
	}
}

void StateLevel::assembleAvatar() {
	Configuration conf = raw.getConfig("avatar");
	avatar = new Avatar();
	avatar->connect(Avatar::BEINGSWALLOWED, this, &StateLevel::handleAvatarBeingSwallowed);
	avatar->pinned = true;
	avatar->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	avatar->speed = r2vec(conf.getReal("speedX"), conf.getReal("speedY"));
	avatar->setElasticity(conf.getReal("k"));
	avatar->setMass(conf.getReal("m"));
	avatar->setCharge(conf.getReal("q"));
	
	// sprite
	avatar->sprite = sprite_avatar;
	((Circle*)avatar->getShape())->setRadius(avatar->sprite->rectW()/2);
}

void StateLevel::assembleBlackHole() {
	Configuration conf = raw.getConfig("blackhole");
	blackhole = new Particle();
	blackhole->pinned = true;
	blackhole->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	blackhole->setMass(conf.getReal("m"));
	
	// sprite
	blackhole->sprite = sprite_blackhole;
	((Circle*)blackhole->getShape())->setRadius(blackhole->sprite->rectW()/2);
}

Item* StateLevel::assembleKey() {
	Configuration conf;
	try {
		conf = raw.getConfig("key");
	}
	catch(Configuration::VarNotFound&) {
		return 0;
	}
	Item* key = new Item();
	key->connect(Item::COLLISION, this, &StateLevel::handleItemCollision);
	key->pinned = true;
	key->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	key->operation = Item::KEY;
	key->setElasticity(conf.getReal("k"));
	key->setMass(conf.getReal("m"));
	
	// sprite
	key->sprite = sprite_key;
	((Circle*)key->getShape())->setRadius(key->sprite->rectW()/2);
	
	return key;
}

Particle* StateLevel::assembleParticle(const Configuration& conf) {
	Particle* particle = new Particle();
	particle->pinned = true;
	particle->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	particle->setElasticity(conf.getReal("k"));
	particle->setMass(conf.getReal("m"));
	particle->setCharge(conf.getReal("q"));
	
	// sprite
	if (particle->getCharge() == 0)
		particle->sprite = sprite_neutral;
	else if (particle->getCharge() < 0) {
		particle->sprite = sprite_negative_anim;
		if (!is_bg_init)
			bg->gradient(particle->getShape()->position.x(0) - bg_x, particle->getShape()->position.x(1) - bg_y, GRADIENT_CONST*ABSVAL(particle->getCharge()), 255, 31, 77, 0);
	}
	else {
		particle->sprite = sprite_positive_anim;
		if (!is_bg_init)
			bg->gradient(particle->getShape()->position.x(0) - bg_x, particle->getShape()->position.x(1) - bg_y, GRADIENT_CONST*ABSVAL(particle->getCharge()), 51, 74, 144, 0);
	}
	((Circle*)particle->getShape())->setRadius(particle->sprite->rectW()/2);
	
	return particle;
}

Item* StateLevel::assembleItem(const Configuration& conf) {
	Item* item = new Item();
	item->connect(Item::COLLISION, this, &StateLevel::handleItemCollision);
	item->pinned = true;
	item->getShape()->position = r2vec(conf.getReal("x"), conf.getReal("y"));
	item->operation = conf.getInt("operation");
	item->value = conf.getReal("value");
	if (item->value < 0)
		item->value *= -1;
	item->setElasticity(conf.getReal("k"));
	item->setMass(conf.getReal("m"));
	
	// sprite
	switch (item->operation) {
	case Item::TIME:
		item->sprite = sprite_item_time;
		break;
		
	case Item::POINT:
		item->sprite = sprite_item_point;
		break;
		
	case Item::LIFE:
		item->sprite = sprite_item_life;
		break;
		
	case Item::MASS:
		item->sprite = sprite_item_mass;
		break;
		
	case Item::BARRIER:
		if (!item->value)
			item->sprite = sprite_item_barrier;
		else
			item->sprite = sprite_item_lethal_barrier;
		break;
		
	default:
		break;
	}
	((Circle*)item->getShape())->setRadius(item->sprite->rectW()/2);
	
	return item;
}

void StateLevel::clear() {
	// the avatar
	delete avatar;
	
	// the blackhole
	delete blackhole;
	
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
	
	// all interactions
	interactions.clear();
	interactions_avatar_item.clear();
}

void StateLevel::setTimeText(int seconds) {
	stringstream ss;
	int minutes = seconds/60;
	seconds -= (minutes*60);
	ss << minutes;
	ss << ":";
	if (seconds < 10)
		ss << "0";
	ss << seconds;
	text_time->setText(ss.str());
}

void StateLevel::handleKeyDown(const observer::Event& event, bool& stop) {
	switch (inputmanager_event.key.keysym.sym) {
	case SDLK_ESCAPE:
		if ((lose_) || (win_))
			return;
		frozen_ = true;
		SDL_ShowCursor(1);
		throw new StackUp("StatePause", (((!life) || (avatar->pinned)) ? new ArgsBase() : 0));
		break;
		
	case SDLK_SPACE:
		if (!avatar->pinned)
			return;
		avatar->pinned = false;
		timer.start(level_time*1000);
		break;
		
	default:
		break;
	}
}

void StateLevel::handleMouseMotion(const observer::Event& event, bool& stop) {
	// handling avatar charge variables
	charge_cursor_position = InputManager::instance()->mouseX();
	if (charge_cursor_position < 640 - MAX_CURSOR_X)
		charge_cursor_position = 640 - MAX_CURSOR_X;
	else if (charge_cursor_position > 640 + MAX_CURSOR_X)
		charge_cursor_position = 640 + MAX_CURSOR_X;
	avatar->setCharge(max_abs_charge*(charge_cursor_position - 640)/MAX_CURSOR_X);
}

void StateLevel::handleTimerDone(const observer::Event& event, bool& stop) {
	lose();
}

void StateLevel::handleAvatarBeingSwallowed(const observer::Event& event, bool& stop) {
	win();
}

void StateLevel::handleItemCollision(const observer::Event& event, bool& stop) {
	if ((lose_) || (win_))
		return;
	
	char operation = ((Item::Event*)&event)->operation;
	int value = ((Item::Event*)&event)->value;
	
	// sprite
	switch (operation) {
	case Item::KEY:
		blackhole->hidden = false;
		interaction_blackhole_force->enabled = true;
		interaction_blackhole_collision->enabled = true;
		break;
		
	case Item::TIME:
		{
			int new_time = round(float(timer.time())/1000) + value;
			if (new_time > 599)
				new_time = 599;
			timer.start(new_time*1000);
		}
		break;
		
	case Item::POINT:
		addPoints(value);
		break;
		
	case Item::LIFE:
		life += value;
		if (life > 3)
			life = 3;
		((Animation*)sprite_life)->setFrame(life);
		break;
		
	case Item::MASS:
		avatar->addMass(value);
		break;
		
	case Item::BARRIER:
		if (value)
			lose();
		break;
		
	default:
		break;
	}
}

void StateLevel::lose() {
	// eatles animation
	eatles = eatles_sheets[3];
	((Animation*)eatles)->setFrame(0);
	eatles_stopwatch.start();
	
	lose_ = true;
	
	// time's up! particles get free and blackhole goes away
	unpinParticles();
	blackhole->hidden = true;
	interaction_blackhole_force->enabled = false;
	interaction_blackhole_collision->enabled = false;
	
	--life;
	if (life >= 0)
		((Animation*)sprite_life)->setFrame(life);
	
	timer.pause();
	
	sound_lose->play(1);
	
	stopwatch.start();
}

void StateLevel::unpinParticles() {
	bg = bg_nograd;
	
	// normal particles
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		(*it)->pinned = false;
		
		// sprite
		if ((*it)->getCharge() < 0)
			(*it)->sprite = sprite_negative;
		else if ((*it)->getCharge() > 0)
			(*it)->sprite = sprite_positive;
	}
	
	// items
	for (list<Item*>::iterator it = items.begin(); it != items.end(); ++it) {
		(*it)->pinned = false;
	}
	
	// changing the avatar-item interactions
	for (list<Interaction*>::iterator it = interactions_avatar_item.begin(); it != interactions_avatar_item.end(); ++it) {
		(*it)->handler = ((Interaction::callback)&Particle::manageParticleCollision);
	}
}

void StateLevel::win() {
	// particles get free
	unpinParticles();
	
	// eatles animation
	eatles = eatles_sheets[4];
	((Animation*)eatles)->setFrame(0);
	eatles_stopwatch.start();
	
	win_ = true;
	
	timer.pause();
	addPoints(round(TIME_POINTS*float(timer.time())/(1000*level_time)) + LIFE_POINTS*life);
	
	sound_win->play(1);
	
	stopwatch.start();
}

void StateLevel::addPoints(int plus) {
	// the new value
	points += plus;
	if (points < 0)
		points = 0;
	else if (points > 9999)
		points = 9999;
	
	// the text
	stringstream ss;
	string tmp;
	ss << points;
	tmp = ss.str();
	text_points->setText(tmp);
	
	// the text size
	switch (tmp.size()) {
		case 1:	text_points->setSize(30);	break;
		case 2:	text_points->setSize(20);	break;
		case 3:	text_points->setSize(15);	break;
		case 4:	text_points->setSize(12);	break;
		default:							break;
	}
}
