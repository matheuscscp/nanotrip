#include <sstream>

#include "common.hpp"

#include "PanelBlackHole.hpp"

#include "Particle.hpp"
#include "SDLBase.hpp"

using namespace common;
using namespace lalge;

using std::stringstream;

PanelBlackHole::PanelBlackHole() {
	sprite = new Sprite("img/levelmaker/panel_blackhole.png");
	setupRectangle();
	
	// inputs
	
	sprite_input_mass = new Sprite("img/levelmaker/input_blackhole_mass.png");
	sprite_input_mass->clip(0, 0, sprite_input_mass->srcW(), sprite_input_mass->srcH()/2);
	button_input_mass = new Button(new Sprite("img/levelmaker/input_blackhole_mass_button.png"));
	button_input_mass->connect(Button::CLICKED, this, &PanelBlackHole::handleInputMassButton);
	input_mass_position = r2vec(105 + button_input_mass->sprite->rectW()/2, 302);
	invalid_input_mass = false;
	input_mass.setMaxSize(20);
	input_mass.set(eval(((Particle*)data->blackhole->getGameObject())->getMass()));
	input_mass.connect(InputString::UPDATE, this, &PanelBlackHole::handleInputMass);
	text_input_mass = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	text_input_mass->setText(input_mass.get());
	
	updatePositions();
}

PanelBlackHole::~PanelBlackHole() {
	delete sprite;
	
	// inputs
	
	delete sprite_input_mass;
	delete button_input_mass->sprite;
	delete button_input_mass;
	delete text_input_mass;
}

void PanelBlackHole::show() {
	// inputs
	button_input_mass->enable(true);
	input_mass.set(eval(((Particle*)data->blackhole->getGameObject())->getMass()));
	
	updatePositions();
}

void PanelBlackHole::hide() {
	// inputs
	button_input_mass->enable(false);
	input_mass.enabled = false;
}

void PanelBlackHole::update() {
	// inputs
	input_mass.update();
	
	updatePositions();
}

void PanelBlackHole::render() {
	if ((invalid_input_mass) || (input_mass.enabled))
		sprite_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1), true);
	text_input_mass->render(button_input_mass->getShape()->position.x(0), button_input_mass->getShape()->position.x(1));
}

void PanelBlackHole::updatePositions() {
	R2Vector origin = getOrigin();
	
	// inputs
	button_input_mass->getShape()->position = origin + input_mass_position;
}

void PanelBlackHole::handleInputMass(const observer::Event& event, bool& stop) {
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
	
	((Particle*)data->blackhole->getGameObject())->setMass(mass);
}

void PanelBlackHole::handleInputMassButton(const observer::Event& event, bool& stop) {
	input_mass.enabled = true;
	handleInputMass(event, stop);
}
