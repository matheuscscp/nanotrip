#include "common.hpp"

#include "PanelKey.hpp"

#include "Particle.hpp"
#include "SDLBase.hpp"

using namespace common;
using namespace lalge;

PanelKey::PanelKey() {
	sprite = new Sprite("img/levelmaker/panel_key.png");
	setupRectangle();
	
	// inputs
	
	sprite_input_mass = new Sprite("img/levelmaker/input_key_mass.png");
	sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	button_input_mass = new Button(new Sprite("img/levelmaker/input_key_mass_button.png"));
	button_input_mass->connect(Button::CLICKED, this, &PanelKey::handleInputMassButton);
	input_mass_position = r2vec(105 + button_input_mass->sprite->rectW()/2, 156 - 29);
	invalid_input_mass = false;
	input_mass.setMaxSize(21);
	input_mass.set(eval(((Particle*)data->key->getGameObject())->getMass()));
	input_mass.connect(InputString::UPDATE, this, &PanelKey::handleInputMass);
	text_input_mass = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_mass->setText(input_mass.get());
	
	sprite_input_elasticity = new Sprite("img/levelmaker/input_key_elasticity.png");
	sprite_input_elasticity->clip(0, 0, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	button_input_elasticity = new Button(new Sprite("img/levelmaker/input_key_elasticity_button.png"));
	button_input_elasticity->connect(Button::CLICKED, this, &PanelKey::handleInputElasticityButton);
	input_elasticity_position = r2vec(159 + button_input_elasticity->sprite->rectW()/2, 156);
	invalid_input_elasticity = false;
	input_elasticity.setMaxSize(15);
	input_elasticity.set(eval(((Particle*)data->key->getGameObject())->getElasticity()));
	input_elasticity.connect(InputString::UPDATE, this, &PanelKey::handleInputElasticity);
	text_input_elasticity = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_elasticity->setText(input_elasticity.get());
	
	updatePositions();
	hide();
}

PanelKey::~PanelKey() {
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
}

void PanelKey::show() {
	// inputs
	button_input_mass->enable(true);
	input_mass.set(eval(((Particle*)data->key->getGameObject())->getMass()));
	button_input_elasticity->enable(true);
	input_elasticity.set(eval(((Particle*)data->key->getGameObject())->getElasticity()));
	
	updatePositions();
}

void PanelKey::hide() {
	// inputs
	button_input_mass->enable(false);
	input_mass.enabled = false;
	button_input_elasticity->enable(false);
	input_elasticity.enabled = false;
}

void PanelKey::update() {
	// inputs
	input_mass.update();
	input_elasticity.update();
	
	updatePositions();
}

void PanelKey::render() {
	if ((invalid_input_mass) || (input_mass.enabled))
		sprite_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1), true);
	text_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1));
	
	if ((invalid_input_elasticity) || (input_elasticity.enabled))
		sprite_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1), true);
	text_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1));
}

void PanelKey::updatePositions() {
	R2Vector origin = getOrigin();
	
	// inputs
	button_input_mass->getShape()->position = origin + input_mass_position;
	button_input_elasticity->getShape()->position = origin + input_elasticity_position;
}

void PanelKey::handleInputMass(const observer::Event& event, bool& stop) {
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
	
	((Particle*)data->key->getGameObject())->setMass(mass);
}

void PanelKey::handleInputMassButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = true;
	input_elasticity.enabled = false;
	handleInputMass(event, stop);
}

void PanelKey::handleInputElasticity(const observer::Event& event, bool& stop) {
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
	
	((Particle*)data->key->getGameObject())->setElasticity(elasticity);
}

void PanelKey::handleInputElasticityButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = false;
	input_elasticity.enabled = true;
	handleInputElasticity(event, stop);
}
