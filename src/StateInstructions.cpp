#include "StateInstructions.hpp"

using namespace lalge;

GAMESTATE_DEF(StateInstructions)

StateInstructions::StateInstructions(ArgsBase* args) {
	bg = new Sprite("img/level/background.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateInstructions::handleGoBack);
}

StateInstructions::~StateInstructions() {
	delete bg;
	
	delete goback->sprite;
	delete goback;
}

void StateInstructions::update() {
	goback->update();
}

void StateInstructions::render() {
	bg->render();
	goback->render();
}

void StateInstructions::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
