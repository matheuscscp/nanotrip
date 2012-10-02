#include <sstream>

#include "common.hpp"

#include "PanelParticle.hpp"

#include "Particle.hpp"
#include "SDLBase.hpp"

using namespace common;
using namespace lalge;

using std::stringstream;
using std::set;

PanelParticle::PanelParticle() {
	sprite = new Sprite("img/levelmaker/panel_particle.png");
	setupRectangle();
	
	// inputs
	
	sprite_input_mass = new Sprite("img/levelmaker/input_particle_mass.png");
	sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	button_input_mass = new Button(new Sprite("img/levelmaker/input_particle_mass_button.png"));
	button_input_mass->connect(Button::CLICKED, this, &PanelParticle::handleInputMassButton);
	input_mass_position = r2vec(105 + button_input_mass->sprite->rectW()/2, 156 - 29);
	invalid_input_mass = false;
	input_mass.setMaxSize(20);
	input_mass.connect(InputString::UPDATE, this, &PanelParticle::handleInputMass);
	text_input_mass = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_mass->setText(input_mass.get());
	
	sprite_input_elasticity = new Sprite("img/levelmaker/input_particle_elasticity.png");
	sprite_input_elasticity->clip(0, 0, sprite_input_elasticity->srcW(), sprite_input_elasticity->srcH()/2);
	button_input_elasticity = new Button(new Sprite("img/levelmaker/input_particle_elasticity_button.png"));
	button_input_elasticity->connect(Button::CLICKED, this, &PanelParticle::handleInputElasticityButton);
	input_elasticity_position = r2vec(159 + button_input_elasticity->sprite->rectW()/2, 156);
	invalid_input_elasticity = false;
	input_elasticity.setMaxSize(15);
	input_elasticity.connect(InputString::UPDATE, this, &PanelParticle::handleInputElasticity);
	text_input_elasticity = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_elasticity->setText(input_elasticity.get());
	
	sprite_input_charge = new Sprite("img/levelmaker/input_particle_charge.png");
	sprite_input_charge->clip(0, 0, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
	button_input_charge = new Button(new Sprite("img/levelmaker/input_particle_charge_button.png"));
	button_input_charge->connect(Button::CLICKED, this, &PanelParticle::handleInputChargeButton);
	input_charge_position = r2vec(105 + 27 + button_input_charge->sprite->rectW()/2, 156 + 29);
	invalid_input_charge = false;
	input_charge.setMaxSize(18);
	input_charge.connect(InputString::UPDATE, this, &PanelParticle::handleInputCharge);
	text_input_charge = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_charge->setText(input_charge.get());
	
	updatePositions();
}

PanelParticle::~PanelParticle() {
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
	
	delete sprite_input_charge;
	delete button_input_charge->sprite;
	delete button_input_charge;
	delete text_input_charge;
}

void PanelParticle::show() {
	// inputs
	button_input_mass->enable(true);
	button_input_elasticity->enable(true);
	button_input_charge->enable(true);
	if (LevelMakerObject::selected.size() == 1) {
		input_mass.set(eval(((Particle*)(*LevelMakerObject::selected.begin())->getGameObject())->getMass()));
		input_elasticity.set(eval(((Particle*)(*LevelMakerObject::selected.begin())->getGameObject())->getElasticity()));
		input_charge.set(eval(((Particle*)(*LevelMakerObject::selected.begin())->getGameObject())->getCharge()));
	}
	else {
		text_input_mass->setText("");
		input_mass.clear();
		text_input_elasticity->setText("");
		input_elasticity.clear();
		text_input_charge->setText("");
		input_charge.clear();
	}
	
	updatePositions();
}

void PanelParticle::hide() {
	// inputs
	button_input_mass->enable(false);
	input_mass.enabled = false;
	button_input_elasticity->enable(false);
	input_elasticity.enabled = false;
	button_input_charge->enable(false);
	input_charge.enabled = false;
}

void PanelParticle::update() {
	// inputs
	input_mass.update();
	input_elasticity.update();
	input_charge.update();
	
	updatePositions();
}

void PanelParticle::render() {
	if ((invalid_input_mass) || (input_mass.enabled))
		sprite_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1), true);
	text_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1));
	
	if ((invalid_input_elasticity) || (input_elasticity.enabled))
		sprite_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1), true);
	text_input_elasticity->render(button_input_elasticity->getShape()->position.x(0), button_input_elasticity->getShape()->position.x(1));
	
	if ((invalid_input_charge) || (input_charge.enabled))
		sprite_input_charge->render(button_input_charge->getShape()->position.x(0), button_input_charge->getShape()->position.x(1), true);
	text_input_charge->render(button_input_charge->getShape()->position.x(0), button_input_charge->getShape()->position.x(1));
}

void PanelParticle::updatePositions() {
	R2Vector origin = getOrigin();
	
	// inputs
	button_input_mass->getShape()->position = origin + input_mass_position;
	button_input_elasticity->getShape()->position = origin + input_elasticity_position;
	button_input_charge->getShape()->position = origin + input_charge_position;
}

void PanelParticle::handleInputMass(const observer::Event& event, bool& stop) {
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
	
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it)
		((Particle*)(*it)->getGameObject())->setMass(mass);
}

void PanelParticle::handleInputMassButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = true;
	input_elasticity.enabled = false;
	input_charge.enabled = false;
}

void PanelParticle::handleInputElasticity(const observer::Event& event, bool& stop) {
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
	
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it)
		((Particle*)(*it)->getGameObject())->setElasticity(elasticity);
}

void PanelParticle::handleInputElasticityButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = false;
	input_elasticity.enabled = true;
	input_charge.enabled = false;
}

void PanelParticle::handleInputCharge(const observer::Event& event, bool& stop) {
	Scalar charge;
	Sprite* new_sprite;
	
	text_input_charge->setText(input_charge.get());
	
	// check invalid input
	if (eval(input_charge.get(), charge)) {
		invalid_input_charge = false;
		sprite_input_charge->clip(0, 0, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
	}
	else {
		charge = 0;
		
		invalid_input_charge = true;
		sprite_input_charge->clip(0, sprite_input_charge->srcH()/2, sprite_input_charge->srcW(), sprite_input_charge->srcH()/2);
	}
	
	// determinating the new sprite
	if (charge < 0)
		new_sprite = LevelMakerData::sprite_negative;
	else if (charge == 0)
		new_sprite = LevelMakerData::sprite_neutral;
	else
		new_sprite = LevelMakerData::sprite_positive;
	
	// updating data
	for (set<LevelMakerObject*>::iterator it = LevelMakerObject::selected.begin(); it != LevelMakerObject::selected.end(); ++it) {
		((Particle*)(*it)->getGameObject())->setCharge(charge);
		(*it)->getGameObject()->sprite = new_sprite;
	}
}

void PanelParticle::handleInputChargeButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = false;
	input_elasticity.enabled = false;
	input_charge.enabled = true;
}
