#include "StateNewGame.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateNewGame)

StateNewGame::StateNewGame(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	play = new Button(new Sprite("img/play.png"));
	play->getShape()->position = r2vec(640, 100);
	play->connect(Button::CLICKED, this, &StateNewGame::handlePlay);
	
	loadlevel = new Button(new Sprite("img/loadlevel.png"));
	loadlevel->getShape()->position = r2vec(640, 200);
	loadlevel->connect(Button::CLICKED, this, &StateNewGame::handleLoadLevel);
	
	makelevel = new Button(new Sprite("img/makelevel.png"));
	makelevel->getShape()->position = r2vec(640, 300);
	makelevel->connect(Button::CLICKED, this, &StateNewGame::handleMakeLevel);
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(640, 400);
	goback->connect(Button::CLICKED, this, &StateNewGame::handleGoBack);
}

void StateNewGame::update() {
}

void StateNewGame::render() {
	GameObject::renderAll();
}

void StateNewGame::handlePlay(const observer::Event& event, bool& stop) {
	throw new Change("StatePlay");
}

void StateNewGame::handleLoadLevel(const observer::Event& event, bool& stop) {
	throw new Change("StateLoadLevel");
}

void StateNewGame::handleMakeLevel(const observer::Event& event, bool& stop) {
	throw new Change("StateMakeLevel");
}

void StateNewGame::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
