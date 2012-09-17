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
	
	loadgame = new Button(new Sprite("img/loadgame.png"));
	loadgame->getShape()->position = r2vec(640, 200);
	loadgame->connect(Button::CLICKED, this, &StateMainMenu::handleLoadGame);
	
	directions = new Button(new Sprite("img/directions.png"));
	directions->getShape()->position = r2vec(640, 300);
	directions->connect(Button::CLICKED, this, &StateMainMenu::handleDirections);
	
	ranking = new Button(new Sprite("img/ranking.png"));
	ranking->getShape()->position = r2vec(640, 400);
	ranking->connect(Button::CLICKED, this, &StateMainMenu::handleRanking);
	
	credits = new Button(new Sprite("img/credits.png"));
	credits->getShape()->position = r2vec(640, 500);
	credits->connect(Button::CLICKED, this, &StateMainMenu::handleCredits);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(640, 600);
	quit->connect(Button::CLICKED, this, &StateMainMenu::handleQuit);
}

void StateMainMenu::update() {
}

void StateMainMenu::render() {
}

void StateMainMenu::handleNewGame(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}

void StateMainMenu::handleLoadGame(const observer::Event& event, bool& stop) {
	SHOW("handleLoadGame");
}

void StateMainMenu::handleDirections(const observer::Event& event, bool& stop) {
	SHOW("handleDirections");
}

void StateMainMenu::handleRanking(const observer::Event& event, bool& stop) {
	SHOW("handleRanking");
}

void StateMainMenu::handleCredits(const observer::Event& event, bool& stop) {
	SHOW("handleCredits");
}
