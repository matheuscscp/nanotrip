#include "StateMainMenu.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateMainMenu)

StateMainMenu::StateMainMenu(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	newgame = new Button(new Sprite("img/newgame.png"));
	newgame->getShape()->position = r2vec(640, 100);
	newgame->connect(Button::CLICKED, this, &StateMainMenu::handleNewGame);
	
	ranking = new Button(new Sprite("img/ranking.png"));
	ranking->getShape()->position = r2vec(640, 200);
	ranking->connect(Button::CLICKED, this, &StateMainMenu::handleRanking);
	
	directions = new Button(new Sprite("img/directions.png"));
	directions->getShape()->position = r2vec(640, 300);
	directions->connect(Button::CLICKED, this, &StateMainMenu::handleDirections);
	
	credits = new Button(new Sprite("img/credits.png"));
	credits->getShape()->position = r2vec(640, 400);
	credits->connect(Button::CLICKED, this, &StateMainMenu::handleCredits);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(640, 500);
	quit->connect(Button::CLICKED, this, &StateMainMenu::handleQuit);
}

void StateMainMenu::update() {
}

void StateMainMenu::render() {
}

void StateMainMenu::handleNewGame(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}

void StateMainMenu::handleRanking(const observer::Event& event, bool& stop) {
	SHOW("handleRanking");
}

void StateMainMenu::handleDirections(const observer::Event& event, bool& stop) {
	SHOW("handleDirections");
}

void StateMainMenu::handleCredits(const observer::Event& event, bool& stop) {
	SHOW("handleCredits");
}
