#include "StateDirections.hpp"

using namespace lalge;

GAMESTATE_DEF(StateDirections)

StateDirections::StateDirections(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateDirections::handleGoBack);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(1180, 680);
	quit->connect(Button::CLICKED, this, &StateDirections::handleQuit);
}

void StateDirections::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
