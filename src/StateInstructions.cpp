#include "StateInstructions.hpp"

using namespace lalge;

GAMESTATE_DEF(StateInstructions)

StateInstructions::StateInstructions(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateInstructions::handleGoBack);
}

void StateInstructions::render() {
	GameObject::renderAll();
}

void StateInstructions::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
