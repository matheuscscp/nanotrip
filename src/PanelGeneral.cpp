#include "common.hpp"

#include "PanelGeneral.hpp"

#include "InputManager.hpp"
#include "Item.hpp"
#include "Circle.hpp"
#include "SDLBase.hpp"

using namespace common;
using namespace lalge;

PanelGeneral::PanelGeneral() {
	sprite = new Sprite("img/levelmaker/panel_general.png");
	setupRectangle();
	
	// borders
	
	border_top = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_top->play_sounds = false;
	border_top_position = r2vec(sprite->rectW()/2, 215);
	border_top->connect(Button::CLICKED, this, &PanelGeneral::handleBorderTop);
	
	border_right = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_right->play_sounds = false;
	border_right_position = r2vec(sprite->rectW() - border_right->sprite->rectW()/2 - 39, 215 + 77);
	border_right->connect(Button::CLICKED, this, &PanelGeneral::handleBorderRight);
	
	border_bottom = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_bottom->play_sounds = false;
	border_bottom_position = r2vec(sprite->rectW()/2, 215 + 77 + 77);
	border_bottom->connect(Button::CLICKED, this, &PanelGeneral::handleBorderBottom);
	
	border_left = new Button(new Sprite("img/levelmaker/icon_border_button.png"));
	border_left->play_sounds = false;
	border_left_position = r2vec(border_left->sprite->rectW()/2 + 39, 215 + 77);
	border_left->connect(Button::CLICKED, this, &PanelGeneral::handleBorderLeft);
	
	// objects
	
	avatar = new Button(new Sprite("img/levelmaker/icon_general_button.png"));
	avatar->play_sounds = false;
	avatar_position = r2vec(sprite->rectW()/2 - 120, 460);
	avatar->connect(Button::CLICKED, this, &PanelGeneral::handleAvatar);
	
	blackhole = new Button(new Sprite("img/levelmaker/icon_general_button.png"));
	blackhole->play_sounds = false;
	blackhole_position = r2vec(avatar_position.x(0) + 60, 460);
	blackhole->connect(Button::CLICKED, this, &PanelGeneral::handleBlackHole);
	
	key = new Button(new Sprite("img/levelmaker/icon_general_button.png"));
	key->play_sounds = false;
	key_position = r2vec(blackhole_position.x(0) + 60, 460);
	key->connect(Button::CLICKED, this, &PanelGeneral::handleKey);
	
	particle = new Button(new Sprite("img/levelmaker/icon_general_button.png"));
	particle->play_sounds = false;
	particle_position = r2vec(key_position.x(0) + 60, 460);
	particle->connect(Button::CLICKED, this, &PanelGeneral::handleParticle);
	
	item = new Button(new Sprite("img/levelmaker/icon_general_button.png"));
	item->play_sounds = false;
	item_position = r2vec(particle_position.x(0) + 60, 460);
	item->connect(Button::CLICKED, this, &PanelGeneral::handleItem);
	
	// inputs
	
	sprite_input_time = new Sprite("img/levelmaker/input_general_time.png");
	sprite_input_time->clip(0, 0, sprite_input_time->srcW(), sprite_input_time->srcH()/2);
	button_input_time = new Button(new Sprite("img/levelmaker/input_general_time_button.png"));
	button_input_time->connect(Button::CLICKED, this, &PanelGeneral::handleInputTimeButton);
	input_time_position = r2vec(110 + button_input_time->sprite->rectW()/2, 64);
	invalid_input_time = false;
	input_time.setMaxSize(3);
	input_time.set(eval(data->level_time));
	input_time.connect(InputString::UPDATE, this, &PanelGeneral::handleInputTime);
	text_input_time = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_time->setText(input_time.get());
	
	sprite_input_charge = new Sprite("img/levelmaker/input_general_charge.png");
	sprite_input_charge->clip(0, 0, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
	button_input_charge = new Button(new Sprite("img/levelmaker/input_general_charge_button.png"));
	button_input_charge->connect(Button::CLICKED, this, &PanelGeneral::handleInputChargeButton);
	input_charge_position = r2vec(200 + button_input_charge->sprite->rectW()/2, 64 + 29);
	invalid_input_charge = false;
	input_charge.setMaxSize(12);
	input_charge.set(eval(data->max_abs_charge));
	input_charge.connect(InputString::UPDATE, this, &PanelGeneral::handleInputCharge);
	text_input_charge = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 12, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_charge->setText(input_charge.get());
	
	sprite_input_bgm = new Sprite("img/levelmaker/input_general_bgm.png");
	sprite_input_bgm->clip(0, 0, sprite_input_bgm->srcW(), sprite_input_bgm->srcH()/2);
	button_input_bgm = new Button(new Sprite("img/levelmaker/input_general_bgm_button.png"));
	button_input_bgm->connect(Button::CLICKED, this, &PanelGeneral::handleInputBGMButton);
	input_bgm_position = r2vec(105 + button_input_bgm->sprite->rectW()/2, 64 + 29 + 29);
	invalid_input_bgm = false;
	input_bgm.setMaxSize(21);
	input_bgm.set(data->bgm);
	input_bgm.connect(InputString::UPDATE, this, &PanelGeneral::handleInputBGM);
	text_input_bgm = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_bgm->setText(input_bgm.get());
	
	updatePositions();
}

PanelGeneral::~PanelGeneral() {
	delete sprite;
	
	// borders
	
	delete border_top->sprite;
	delete border_top;
	
	delete border_right->sprite;
	delete border_right;
	
	delete border_bottom->sprite;
	delete border_bottom;
	
	delete border_left->sprite;
	delete border_left;
	
	// objects
	
	delete avatar->sprite;
	delete avatar;
	
	delete blackhole->sprite;
	delete blackhole;
	
	delete key->sprite;
	delete key;
	
	delete particle->sprite;
	delete particle;
	
	delete item->sprite;
	delete item;
	
	// inputs
	
	delete sprite_input_time;
	delete button_input_time->sprite;
	delete button_input_time;
	delete text_input_time;
	
	delete sprite_input_charge;
	delete button_input_charge->sprite;
	delete button_input_charge;
	delete text_input_charge;
	
	delete sprite_input_bgm;
	delete button_input_bgm->sprite;
	delete button_input_bgm;
	delete text_input_bgm;
}

void PanelGeneral::show() {
	// borders
	border_top->enable(true);
	border_right->enable(true);
	border_bottom->enable(true);
	border_left->enable(true);
	
	// objects
	avatar->enable(true);
	blackhole->enable(true);
	key->enable(true);
	particle->enable(true);
	item->enable(true);
	
	// inputs
	button_input_time->enable(true);
	input_time.set(eval(data->level_time));
	button_input_charge->enable(true);
	input_charge.set(eval(data->max_abs_charge));
	button_input_bgm->enable(true);
	input_bgm.set(data->bgm);
	
	updatePositions();
}

void PanelGeneral::hide() {
	// borders
	border_top->enable(false);
	border_right->enable(false);
	border_bottom->enable(false);
	border_left->enable(false);
	
	// objects
	avatar->enable(false);
	blackhole->enable(false);
	key->enable(false);
	particle->enable(false);
	item->enable(false);
	
	// inputs
	button_input_time->enable(false);
	input_time.enabled = false;
	button_input_charge->enable(false);
	input_charge.enabled = false;
	button_input_bgm->enable(false);
	input_bgm.enabled = false;
}

void PanelGeneral::update() {
	// inputs
	input_time.update();
	input_charge.update();
	input_bgm.update();
	
	updatePositions();
}

void PanelGeneral::render() {
	if ((invalid_input_time) || (input_time.enabled))
		sprite_input_time->render(button_input_time->getShape()->position.x(0), button_input_time->getShape()->position.x(1), true);
	text_input_time->render(button_input_time->getShape()->position.x(0), button_input_time->getShape()->position.x(1));
	
	if ((invalid_input_charge) || (input_charge.enabled))
		sprite_input_charge->render(button_input_charge->getShape()->position.x(0), button_input_charge->getShape()->position.x(1), true);
	text_input_charge->render(button_input_charge->getShape()->position.x(0), button_input_charge->getShape()->position.x(1));
	
	if ((invalid_input_bgm) || (input_bgm.enabled))
		sprite_input_bgm->render(button_input_bgm->getShape()->position.x(0), button_input_bgm->getShape()->position.x(1), true);
	text_input_bgm->render(button_input_bgm->getShape()->position.x(0), button_input_bgm->getShape()->position.x(1));
}

void PanelGeneral::updatePositions() {
	R2Vector origin = getOrigin();
	
	// borders
	border_top->getShape()->position = origin + border_top_position;
	border_right->getShape()->position = origin + border_right_position;
	border_bottom->getShape()->position = origin + border_bottom_position;
	border_left->getShape()->position = origin + border_left_position;
	
	// objects
	avatar->getShape()->position = origin + avatar_position;
	blackhole->getShape()->position = origin + blackhole_position;
	key->getShape()->position = origin + key_position;
	particle->getShape()->position = origin + particle_position;
	item->getShape()->position = origin + item_position;
	
	// inputs
	button_input_time->getShape()->position = origin + input_time_position;
	button_input_charge->getShape()->position = origin + input_charge_position;
	button_input_bgm->getShape()->position = origin + input_bgm_position;
}

void PanelGeneral::handleBorderTop(const observer::Event& event, bool& stop) {
	data->has_top = (!data->has_top);
}

void PanelGeneral::handleBorderRight(const observer::Event& event, bool& stop) {
	data->has_right = (!data->has_right);
}

void PanelGeneral::handleBorderBottom(const observer::Event& event, bool& stop) {
	data->has_bottom = (!data->has_bottom);
}

void PanelGeneral::handleBorderLeft(const observer::Event& event, bool& stop) {
	data->has_left = (!data->has_left);
}

void PanelGeneral::handleAvatar(const observer::Event& event, bool& stop) {
	// select avatar
	LevelMakerObject::selected.clear();
	LevelMakerObject::selected.insert(data->avatar);
}

void PanelGeneral::handleBlackHole(const observer::Event& event, bool& stop) {
	// select blackhole
	LevelMakerObject::selected.clear();
	LevelMakerObject::selected.insert(data->blackhole);
}

void PanelGeneral::handleKey(const observer::Event& event, bool& stop) {
	// select if key already exists
	if (data->key) {
		LevelMakerObject::selected.clear();
		LevelMakerObject::selected.insert(data->key);
	}
	// create key
	else {
		Item* key_obj = new Item();
		key_obj->getShape()->position = r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
		
		// sprite
		key_obj->sprite = LevelMakerData::sprite_key;
		((Circle*)key_obj->getShape())->setRadius(key_obj->sprite->rectW()/2);
		
		LevelMakerObject::creating = new LevelMakerObject(LevelMakerObject::KEY, key_obj);
		LevelMakerObject::creating->selection = LevelMakerData::sprite_key_selection;
	}
}

void PanelGeneral::handleParticle(const observer::Event& event, bool& stop) {
	// create particle
	Particle* particle_obj = new Particle();
	particle_obj->getShape()->position = r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
	
	// sprite
	particle_obj->sprite = LevelMakerData::sprite_neutral;
	((Circle*)particle_obj->getShape())->setRadius(particle_obj->sprite->rectW()/2);
	
	LevelMakerObject::creating = new LevelMakerObject(LevelMakerObject::PARTICLE, particle_obj);
	LevelMakerObject::creating->selection = LevelMakerData::sprite_particle_selection;
}

void PanelGeneral::handleItem(const observer::Event& event, bool& stop) {
	Item* item_obj = new Item();
	item_obj->getShape()->position = r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
	item_obj->operation = Item::TIME;
	item_obj->setValue(10);
	
	// sprite
	item_obj->sprite = LevelMakerData::sprite_item_time;
	((Circle*)item_obj->getShape())->setRadius(item_obj->sprite->rectW()/2);
	
	LevelMakerObject::creating = new LevelMakerObject(LevelMakerObject::ITEM, item_obj);
	LevelMakerObject::creating->selection = LevelMakerData::sprite_item_time_selection;
}

void PanelGeneral::handleInputTime(const observer::Event& event, bool& stop) {
	int level_time;
	
	text_input_time->setText(input_time.get());
	
	// check invalid input
	if (eval(input_time.get(), level_time)) {
		if ((level_time < 5) || (level_time > 599)) {
			invalid_input_time = true;
			sprite_input_time->clip(0, sprite_input_time->srcH()/2, sprite_input_time->srcW(), sprite_input_time->srcH()/2);
			return;
		}
		data->level_time = level_time;
		
		invalid_input_time = false;
		sprite_input_time->clip(0, 0, sprite_input_time->srcW(), sprite_input_time->srcH()/2);
	}
	else {
		invalid_input_time = true;
		sprite_input_time->clip(0, sprite_input_time->srcH()/2, sprite_input_time->srcW(), sprite_input_time->srcH()/2);
	}
}

void PanelGeneral::handleInputTimeButton(const observer::Event& event, bool& stop) {
	input_time.enabled = true;
	input_charge.enabled = false;
	input_bgm.enabled = false;
	handleInputTime(event, stop);
}

void PanelGeneral::handleInputCharge(const observer::Event& event, bool& stop) {
	lalge::Scalar max_abs_charge;
	
	text_input_charge->setText(input_charge.get());
	
	// check invalid input
	if (eval(input_charge.get(), max_abs_charge)) {
		if (max_abs_charge <= 0) {
			invalid_input_charge = true;
			sprite_input_charge->clip(0, sprite_input_charge->srcH()/2, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
			return;
		}
		data->max_abs_charge = max_abs_charge;
		
		invalid_input_charge = false;
		sprite_input_charge->clip(0, 0, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
	}
	else {
		invalid_input_charge = true;
		sprite_input_charge->clip(0, sprite_input_charge->srcH()/2, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
	}
}

void PanelGeneral::handleInputChargeButton(const observer::Event& event, bool& stop) {
	input_time.enabled = false;
	input_charge.enabled = true;
	input_bgm.enabled = false;
	handleInputCharge(event, stop);
}

void PanelGeneral::handleInputBGM(const observer::Event& event, bool& stop) {
	text_input_bgm->setText(input_bgm.get());
	
	// empty bgm
	if (!input_bgm.get().size()) {
		data->bgm = "";
		
		invalid_input_bgm = false;
		sprite_input_bgm->clip(0, 0, sprite_input_bgm->srcW(), sprite_input_bgm->srcH()/2);
		return;
	}
	
	// check whether file exists
	try {
		Audio audio_test("sfx/level/" + input_bgm.get() + ".mp3");
		
		data->bgm = input_bgm.get();
		
		invalid_input_bgm = false;
		sprite_input_bgm->clip(0, 0, sprite_input_bgm->srcW(), sprite_input_bgm->srcH()/2);
	}
	catch (mexception&) {
		invalid_input_bgm = true;
		sprite_input_bgm->clip(0, sprite_input_bgm->srcH()/2, sprite_input_bgm->srcW(), sprite_input_bgm->srcH()/2);
	}
}

void PanelGeneral::handleInputBGMButton(const observer::Event& event, bool& stop) {
	input_time.enabled = false;
	input_charge.enabled = false;
	input_bgm.enabled = true;
	handleInputBGM(event, stop);
}
