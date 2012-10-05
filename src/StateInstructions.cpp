#include "StateInstructions.hpp"

using namespace lalge;

GAMESTATE_DEF(StateInstructions)

StateInstructions::StateInstructions(ArgsBase* args) {
	bg = new Sprite("img/menus/background.png");
	instructions = new Sprite("img/menus/instructions.png");
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(640, 600);
	back->connect(Button::CLICKED, this, &StateInstructions::handleGoBack);
}

StateInstructions::~StateInstructions() {
	delete bg;
	delete instructions;
	
	delete back->sprite;
	delete back;
}

void StateInstructions::update() {
	back->update();
}

void StateInstructions::render() {
	bg->render();
	instructions->render();
	
	back->render();
}

void StateInstructions::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
