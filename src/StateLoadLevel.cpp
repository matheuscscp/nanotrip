#include "StateLoadLevel.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateLoadLevel)

StateLoadLevel::StateLoadLevel(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	inputbox = new Sprite("img/inputlevelname.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(540, 500);
	goback->connect(Button::CLICKED, this, &StateLoadLevel::handleGoBack);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(740, 500);
	quit->connect(Button::CLICKED, this, &StateLoadLevel::handleQuit);
}

void StateLoadLevel::update() {
}

void StateLoadLevel::render() {
	inputbox->render(640, 300, true);
}

void StateLoadLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
