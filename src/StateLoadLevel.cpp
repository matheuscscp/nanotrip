#include "StateLoadLevel.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateLoadLevel)

StateLoadLevel::StateLoadLevel(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	history = new Button(new Sprite("img/load_button.png"));
	history->getShape()->position = r2vec(640, 100);
	history->connect(Button::CLICKED, this, &StateLoadLevel::handleHistory);
	
	loadlevel = new Button(new Sprite("img/load_button.png"));
	loadlevel->getShape()->position = r2vec(840, 200);
	loadlevel->connect(Button::CLICKED, this, &StateLoadLevel::handleLoadLevel);
	
	makelevel = new Button(new Sprite("img/load_button.png"));
	makelevel->getShape()->position = r2vec(640, 300);
	makelevel->connect(Button::CLICKED, this, &StateLoadLevel::handleMakeLevel);
	
	goback = new Button(new Sprite("img/load_button.png"));
	goback->getShape()->position = r2vec(640, 400);
	goback->connect(Button::CLICKED, this, &StateLoadLevel::handleGoBack);
}

void StateLoadLevel::update() {
}

void StateLoadLevel::render() {
}

void StateLoadLevel::handleHistory(const observer::Event& event, bool& stop) {
	throw new Change("StateFirstLevel");
}

void StateLoadLevel::handleLoadLevel(const observer::Event& event, bool& stop) {
	throw new Change("StateLoadLevel");
}

void StateLoadLevel::handleMakeLevel(const observer::Event& event, bool& stop) {
	SHOW("makelevel");
}

void StateLoadLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw Quit();
}
