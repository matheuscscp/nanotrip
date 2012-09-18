#include "StateInstructions.hpp"

using namespace lalge;

GAMESTATE_DEF(StateInstructions)

StateInstructions::StateInstructions(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateInstructions::handleGoBack);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(1180, 680);
	quit->connect(Button::CLICKED, this, &StateInstructions::handleQuit);
}

void StateInstructions::render() {
	GameObject::renderAll();
}

void StateInstructions::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
