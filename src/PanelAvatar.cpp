#include <sstream>

#include "common.hpp"

#include "PanelAvatar.hpp"

#include "Particle.hpp"
#include "SDLBase.hpp"
#include "InputManager.hpp"

using namespace common;
using namespace lalge;

using std::stringstream;

PanelAvatar::PanelAvatar() {
	sprite = new Sprite("img/levelmaker/panel_avatar.png");
	setupRectangle();
	
	// inputs
	
	sprite_input_mass = new Sprite("img/levelmaker/input_avatar_mass.png");
	sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	button_input_mass = new Button(new Sprite("img/levelmaker/input_avatar_mass_button.png"));
	button_input_mass->connect(Button::CLICKED, this, &PanelAvatar::handleInputMassButton);
	input_mass_position = r2vec(105 + button_input_mass->sprite->rectW()/2, 172 - 29);
	invalid_input_mass = false;
	input_mass.setMaxSize(20);
	input_mass.set(eval(((Particle*)data->avatar->getGameObject())->getMass()));
	input_mass.connect(InputString::UPDATE, this, &PanelAvatar::handleInputMass);
	text_input_mass = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_mass->setText(input_mass.get());
	
	sprite_input_elasticity = new Sprite("img/levelmaker/input_avatar_elasticity.png");
	sprite_input_elasticity->clip(0, 0, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	button_input_elasticity = new Button(new Sprite("img/levelmaker/input_avatar_elasticity_button.png"));
	button_input_elasticity->connect(Button::CLICKED, this, &PanelAvatar::handleInputElasticityButton);
	input_elasticity_position = r2vec(159 + button_input_elasticity->sprite->rectW()/2, 172);
	invalid_input_elasticity = false;
	input_elasticity.setMaxSize(15);
	input_elasticity.set(eval(((Particle*)data->avatar->getGameObject())->getElasticity()));
	input_elasticity.connect(InputString::UPDATE, this, &PanelAvatar::handleInputElasticity);
	text_input_elasticity = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_elasticity->setText(input_elasticity.get());
	
	// set speed button
	set_speed = new Button(new Sprite("img/levelmaker/button_set_speed.png"));
	set_speed->connect(Button::CLICKED, this, &PanelAvatar::handleSetSpeed);
	set_speed_position = r2vec(sprite->rectW()/2, 240);
	
	updatePositions();
}

PanelAvatar::~PanelAvatar() {
	delete sprite;
	
	// inputs
	
	delete sprite_input_mass;
	delete button_input_mass->sprite;
	delete button_input_mass;
	delete text_input_mass;
	
	delete sprite_input_elasticity;
	delete button_input_elasticity->sprite;
	delete button_input_elasticity;
	delete text_input_elasticity;
	
	// set speed button
	delete set_speed->sprite;
	delete set_speed;
}

void PanelAvatar::show() {
	// inputs
	button_input_mass->enable(true);
	input_mass.set(eval(((Particle*)data->avatar->getGameObject())->getMass()));
	button_input_elasticity->enable(true);
	input_elasticity.set(eval(((Particle*)data->avatar->getGameObject())->getElasticity()));
	
	// set speed button
	set_speed->enable(true);
	
	updatePositions();
}

void PanelAvatar::hide() {
	// inputs
	button_input_mass->enable(false);
	input_mass.enabled = false;
	button_input_elasticity->enable(false);
	input_elasticity.enabled = false;
	
	// set speed button
	set_speed->enable(false);
}

void PanelAvatar::update() {
	// inputs
	input_mass.update();
	input_elasticity.update();
	
	// set speed
	set_speed->update();
	if (LevelMakerObject::setting_speed)
		((Particle*)data->avatar->getGameObject())->speed = r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY()) - data->avatar->getShape()->position;
	
	updatePositions();
}

void PanelAvatar::render() {
	// inputs
	
	if ((invalid_input_mass) || (input_mass.enabled))
		sprite_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1), true);
	text_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1));
	
	if ((invalid_input_elasticity) || (input_elasticity.enabled))
		sprite_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1), true);
	text_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1));
	
	// set speed button
	set_speed->render();
}

void PanelAvatar::updatePositions() {
	R2Vector origin = getOrigin();
	
	// inputs
	button_input_mass->getShape()->position = origin + input_mass_position;
	button_input_elasticity->getShape()->position = origin + input_elasticity_position;
	
	// set speed button
	set_speed->getShape()->position = origin + set_speed_position;
}

void PanelAvatar::handleInputMass(const observer::Event& event, bool& stop) {
	Scalar mass;
	
	text_input_mass->setText(input_mass.get());
	
	// check invalid input
	if (eval(input_mass.get(), mass)) {
		mass = ((mass <= 0) ? 1 : mass);
		
		invalid_input_mass = false;
		sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	}
	else {
		mass = 1;
		
		invalid_input_mass = true;
		sprite_input_mass->clip(0, sprite_input_mass->srcH()/2, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	}
	
	((Particle*)data->avatar->getGameObject())->setMass(mass);
}

void PanelAvatar::handleInputMassButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = true;
	input_elasticity.enabled = false;
	handleInputMass(event, stop);
}

void PanelAvatar::handleInputElasticity(const observer::Event& event, bool& stop) {
	Scalar elasticity;
	
	text_input_elasticity->setText(input_elasticity.get());
	
	// check invalid input
	if (eval(input_elasticity.get(), elasticity)) {
		elasticity = ((elasticity > 0.5) ? 0.5 : elasticity);
		elasticity = ((elasticity < 0) ? 0 : elasticity);
		
		invalid_input_elasticity = false;
		sprite_input_elasticity->clip(0, 0, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	}
	else {
		elasticity = 0;
		
		invalid_input_elasticity = true;
		sprite_input_elasticity->clip(0, sprite_input_elasticity->srcH()/2, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	}
	
	((Particle*)data->avatar->getGameObject())->setElasticity(elasticity);
}

void PanelAvatar::handleInputElasticityButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = false;
	input_elasticity.enabled = true;
	handleInputElasticity(event, stop);
}

void PanelAvatar::handleSetSpeed(const observer::Event& event, bool& stop) {
	SDL_WarpMouse(data->avatar->getShape()->position.x(0), data->avatar->getShape()->position.x(1));
	LevelMakerObject::setting_speed = true;
}
