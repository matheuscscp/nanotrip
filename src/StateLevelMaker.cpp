#include "StateLevelMaker.hpp"

#include "InputManager.hpp"
#include "StateLevel.hpp"
#include "LevelMakerPanel.hpp"
#include "SDLBase.hpp"

using namespace common;
using namespace lalge;

using std::list;

GAMESTATE_DEF(StateLevelMaker)

StateLevelMaker::UnstackArgs::UnstackArgs(int op) : op(op) {}

StateLevelMaker::Args::Args(const std::string& levelname) : levelname(levelname) {}

StateLevelMaker::StateLevelMaker(ArgsBase* args) : clicking_button(false) {
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
	LevelMakerData::sprite_avatar = new Sprite("img/levelmaker/avatar.png");
	LevelMakerData::sprite_blackhole = new Sprite("img/levelmaker/blackhole.png");
	LevelMakerData::sprite_key = new Sprite("img/levelmaker/item_key.png");
	LevelMakerData::sprite_negative = new Sprite("img/levelmaker/particle_negative.png");
	LevelMakerData::sprite_neutral = new Sprite("img/levelmaker/particle_neutral.png");
	LevelMakerData::sprite_positive = new Sprite("img/levelmaker/particle_positive.png");
	LevelMakerData::sprite_item_time = new Sprite("img/levelmaker/item_time.png");
	LevelMakerData::sprite_item_point = new Sprite("img/levelmaker/item_point.png");
	LevelMakerData::sprite_item_life = new Sprite("img/levelmaker/item_life.png");
	LevelMakerData::sprite_item_mass = new Sprite("img/levelmaker/item_mass.png");
	LevelMakerData::sprite_item_barrier = new Sprite("img/levelmaker/item_bounce_star.png");
	LevelMakerData::sprite_item_lethal_barrier = new Sprite("img/levelmaker/item_death_star.png");
	LevelMakerData::sprite_avatar_selection = new Sprite("img/levelmaker/avatar_selection.png");
	LevelMakerData::sprite_blackhole_selection = new Sprite("img/levelmaker/blackhole_selection.png");
	LevelMakerData::sprite_key_selection = new Sprite("img/levelmaker/item_key_selection.png");
	LevelMakerData::sprite_particle_selection = new Sprite("img/levelmaker/particle_selection.png");
	LevelMakerData::sprite_item_time_selection = new Sprite("img/levelmaker/item_time_selection.png");
	LevelMakerData::sprite_item_point_selection = new Sprite("img/levelmaker/item_point_selection.png");
	LevelMakerData::sprite_item_life_selection = new Sprite("img/levelmaker/item_life_selection.png");
	LevelMakerData::sprite_item_mass_selection = new Sprite("img/levelmaker/item_mass_selection.png");
	LevelMakerData::sprite_item_barrier_selection = new Sprite("img/levelmaker/item_bounce_star_selection.png");
	LevelMakerData::sprite_item_lethal_barrier_selection = new Sprite("img/levelmaker/item_death_star_selection.png");
	border_top = new Sprite("img/levelmaker/border_top.png");
	border_right = new Sprite("img/levelmaker/border_right.png");
	border_bottom = new Sprite("img/levelmaker/border_bottom.png");
	border_left = new Sprite("img/levelmaker/border_left.png");
	LevelMakerObject::sprite_selection_box = new Sprite("img/levelmaker/selection_box.png");
	LevelMakerObject::sprite_selection_horizontal = new Sprite("img/levelmaker/selection_horizontal.png");
	LevelMakerObject::sprite_selection_vertical = new Sprite("img/levelmaker/selection_vertical.png");
	
	// all buttons
	
	button_quit = new Button(new Sprite("img/levelmaker/button_quit.png"));
	button_quit->getShape()->position = r2vec(60, 50);
	button_quit->connect(Button::CLICKED, this, &StateLevelMaker::handleQuitButton);
	button_quit->update();
	
	button_save = new Button(new Sprite("img/levelmaker/button_save.png"));
	button_save->getShape()->position = r2vec(160, 50);
	button_save->connect(Button::CLICKED, this, &StateLevelMaker::handleSave);
	button_save->update();
	
	button_revert = new Button(new Sprite("img/levelmaker/button_revert.png"));
	button_revert->getShape()->position = r2vec(60, 100);
	button_revert->connect(Button::CLICKED, this, &StateLevelMaker::handleRevert);
	button_revert->update();
	
	button_test = new Button(new Sprite("img/levelmaker/button_test.png"));
	button_test->getShape()->position = r2vec(160, 100);
	button_test->connect(Button::CLICKED, this, &StateLevelMaker::handleTest);
	button_test->update();
	
	button_delete = new Button(new Sprite("img/levelmaker/button_delete.png"));
	button_delete->getShape()->position = r2vec(60, 150);
	button_delete->connect(Button::CLICKED, this, &StateLevelMaker::handleDelete);
	button_delete->update();
	
	button_clone = new Button(new Sprite("img/levelmaker/button_clone.png"));
	button_clone->getShape()->position = r2vec(160, 150);
	button_clone->connect(Button::CLICKED, this, &StateLevelMaker::handleClone);
	button_clone->update();
	
	// data
	if (dynamic_cast<Args*>(args))
		data = new LevelMakerData(((Args*)args)->levelname);
	else if (dynamic_cast<StateLevel::FinalArgs*>(args))
		data = new LevelMakerData(((Args*)((StateLevel::FinalArgs*)args)->nextargs)->levelname);
	delete args;
	
	LevelMakerPanel::init(data);
}

StateLevelMaker::~StateLevelMaker() {
	delete bg;
	delete hud;
	
	// all sprites
	delete LevelMakerData::sprite_avatar;
	delete LevelMakerData::sprite_blackhole;
	delete LevelMakerData::sprite_key;
	delete LevelMakerData::sprite_negative;
	delete LevelMakerData::sprite_neutral;
	delete LevelMakerData::sprite_positive;
	delete LevelMakerData::sprite_item_time;
	delete LevelMakerData::sprite_item_point;
	delete LevelMakerData::sprite_item_life;
	delete LevelMakerData::sprite_item_mass;
	delete LevelMakerData::sprite_item_barrier;
	delete LevelMakerData::sprite_item_lethal_barrier;
	delete LevelMakerData::sprite_avatar_selection;
	delete LevelMakerData::sprite_blackhole_selection;
	delete LevelMakerData::sprite_key_selection;
	delete LevelMakerData::sprite_particle_selection;
	delete LevelMakerData::sprite_item_time_selection;
	delete LevelMakerData::sprite_item_point_selection;
	delete LevelMakerData::sprite_item_life_selection;
	delete LevelMakerData::sprite_item_mass_selection;
	delete LevelMakerData::sprite_item_barrier_selection;
	delete LevelMakerData::sprite_item_lethal_barrier_selection;
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
	
	delete button_clone->sprite;
	delete button_clone;
	
	delete data;
	
	LevelMakerPanel::close();
}

void StateLevelMaker::handleUnstack(ArgsBase* args) {
	frozen_ = false;
	
	if (!args)
		return;
	
	int op = ((UnstackArgs*)args)->op;
	delete args;
	
	switch (op) {
	case UnstackArgs::SAVE_MENU:
		data->save();
		throw new Change("StateMakeLevel", new Args(data->levelname));
		
	case UnstackArgs::SAVE_QUIT:
		data->save();
		throw Quit();
		
	case UnstackArgs::MENU:
		throw new Change("StateMakeLevel", new Args(data->levelname));
		
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
	
	if ((LevelMakerPanel::mouseInside()) ||
		(LevelMakerObject::creating) ||
		(LevelMakerObject::setting_speed)) {
		disableButtons();
		return;
	}
	else
		enableButtons();
	
	button_save->update();
	button_revert->update();
	button_test->update();
	button_quit->update();
	button_delete->update();
	button_clone->update();
}

void StateLevelMaker::render() {
	bg->render(bg_x, bg_y);
	
	// borders
	if (data->has_top)
		border_top->render(30, 0);
	if (data->has_right)
		border_right->render(1250, 0);
	if (data->has_bottom)
		border_bottom->render(0, 666);
	if (data->has_left)
		border_left->render();
	
	// hud
	hud->render();
	button_save->render();
	button_revert->render();
	button_test->render();
	button_quit->render();
	button_delete->render();
	button_clone->render();
	
	data->blackhole->render();
	
	// all particles
	for (list<LevelMakerObject*>::iterator it = data->particles.begin(); it != data->particles.end(); ++it)
		(*it)->render();
	
	// key
	if (data->key)
		data->key->render();
	
	// all items
	for (list<LevelMakerObject*>::iterator it = data->items.begin(); it != data->items.end(); ++it)
		(*it)->render();
	
	data->avatar->render();
	renderAvatarSpeed();
	
	LevelMakerObject::renderSelection();
	
	// panel
	LevelMakerPanel::renderCurrent();
}

void StateLevelMaker::renderAvatarSpeed() {
	R2Vector speed = ((Particle*)data->avatar->getGameObject())->speed;
	if (!speed.size())
		return;
	
	R2Vector beg = data->avatar->getShape()->position;
	R2Vector end = beg + speed;
	
	SDLBase::drawLine(beg, end, 0xFFFFFF, 0, 2);
	SDLBase::drawLine(end, end + rotate(150, speed.unitvec()*speed.size()/4.8), 0xFFFFFF, 0, 2);
	SDLBase::drawLine(end, end + rotate(-150, speed.unitvec()*speed.size()/4.8), 0xFFFFFF, 0, 2);
}

void StateLevelMaker::disableButtons() {
	button_save->enable(false);
	button_save->sprite->clip(0, 0, button_save->sprite->srcW(), button_save->sprite->srcH()/4);
	button_revert->enable(false);
	button_revert->sprite->clip(0, 0, button_revert->sprite->srcW(), button_revert->sprite->srcH()/4);
	button_test->enable(false);
	button_test->sprite->clip(0, 0, button_test->sprite->srcW(), button_test->sprite->srcH()/4);
	button_quit->enable(false);
	button_quit->sprite->clip(0, 0, button_quit->sprite->srcW(), button_quit->sprite->srcH()/4);
	button_delete->enable(false);
	button_delete->sprite->clip(0, 0, button_delete->sprite->srcW(), button_delete->sprite->srcH()/4);
	button_clone->enable(false);
	button_clone->sprite->clip(0, 0, button_clone->sprite->srcW(), button_clone->sprite->srcH()/4);
}

void StateLevelMaker::enableButtons() {
	button_save->enable(true);
	button_revert->enable(true);
	button_test->enable(true);
	button_quit->enable(true);
	button_delete->enable(true);
	button_clone->enable(true);
}

void StateLevelMaker::handleSave(const observer::Event& event, bool& stop) {
	data->save();
}

void StateLevelMaker::handleRevert(const observer::Event& event, bool& stop) {
	data->revert();
	LevelMakerPanel::updateCurrent(true);
}

void StateLevelMaker::handleTest(const observer::Event& event, bool& stop) {
	data->save();
	throw new Change("StateLevel", new StateLevel::Args(data->levelname, "StateLevelMaker", new Args(data->levelname)));
}

void StateLevelMaker::handleQuitButton(const observer::Event& event, bool& stop) {
	if (!data->modified())
		throw new Change("StateMakeLevel", new Args(data->levelname));
	
	frozen_ = true;
	throw new StackUp("StateLevelMakerQuit");
}

void StateLevelMaker::handleDelete(const observer::Event& event, bool& stop) {
	data->deleteSelection();
}

void StateLevelMaker::handleClone(const observer::Event& event, bool& stop) {
	data->cloneSelection();
}

void StateLevelMaker::handleKeyDown(const observer::Event& event, bool& stop) {
	switch (inputmanager_event.key.keysym.sym) {
	case SDLK_s:
	case 'S':
		if ((InputManager::instance()->keyPressed(SDLK_RCTRL)) ||
			(InputManager::instance()->keyPressed(SDLK_LCTRL))) {
			handleSave(event, stop);
		}
		break;
		
	case SDLK_r:
	case 'R':
		if ((InputManager::instance()->keyPressed(SDLK_RCTRL)) ||
			(InputManager::instance()->keyPressed(SDLK_LCTRL))) {
			handleRevert(event, stop);
		}
		break;
		
	case SDLK_t:
	case 'T':
		if ((InputManager::instance()->keyPressed(SDLK_RCTRL)) ||
			(InputManager::instance()->keyPressed(SDLK_LCTRL))) {
			handleTest(event, stop);
		}
		break;
		
	case SDLK_ESCAPE:
		handleQuitButton(event, stop);
		// no break. throws a new game state
		
	case SDLK_DELETE:
		handleDelete(event, stop);
		break;
		
	case SDLK_c:
	case 'C':
		if ((InputManager::instance()->keyPressed(SDLK_RCTRL)) ||
			(InputManager::instance()->keyPressed(SDLK_LCTRL))) {
			handleClone(event, stop);
		}
		break;
		
	default:
		break;
	}
}

void StateLevelMaker::handleQuit(const observer::Event& event, bool& stop) {
	if (!data->modified())
		throw Quit();
	
	frozen_ = true;
	throw new StackUp("StateLevelMakerQuit", new ArgsBase());
}

void StateLevelMaker::checkSelectionRequests() {
	if (clicking_button) {
		LevelMakerObject::deselection_requested = false;
		LevelMakerObject::selection_requested = false;
		LevelMakerObject::toggle_requested = false;
		// don't cancel mousedown request.
		// you'll can't move things that are inside the hud
	}
}

void StateLevelMaker::handleMouseLeft(const observer::Event& event, bool& stop) {
	clicking_button = false;
	if ((button_save->getShape()->mouseDownInside()) ||
		(button_revert->getShape()->mouseDownInside()) ||
		(button_test->getShape()->mouseDownInside()) ||
		(button_quit->getShape()->mouseDownInside()) ||
		(button_delete->getShape()->mouseDownInside()) ||
		(button_clone->getShape()->mouseDownInside())) {
		clicking_button = true;
	}
}
