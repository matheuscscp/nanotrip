#include "StateNewGame.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateNewGame)

StateNewGame::StateNewGame(ArgsBase* args) {
	bg = new Sprite("img/level/background.png");
	
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

StateNewGame::~StateNewGame() {
	delete bg;
	
	delete play->sprite;
	delete play;
	
	delete loadlevel->sprite;
	delete loadlevel;
	
	delete makelevel->sprite;
	delete makelevel;
	
	delete goback->sprite;
	delete goback;
}

void StateNewGame::update() {
	play->update();
	loadlevel->update();
	makelevel->update();
	goback->update();
}

void StateNewGame::render() {
	bg->render();
	
	play->render();
	loadlevel->render();
	makelevel->render();
	goback->render();
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
