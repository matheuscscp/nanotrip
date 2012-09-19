#include "StateMainMenu.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateMainMenu)

StateMainMenu::StateMainMenu(ArgsBase* args) {
	bg = new Sprite("img/level/background.png");
	
	newgame = new Button(new Sprite("img/newgame.png"));
	newgame->getShape()->position = r2vec(640, 100);
	newgame->connect(Button::CLICKED, this, &StateMainMenu::handleNewGame);
	
	ranking = new Button(new Sprite("img/ranking.png"));
	ranking->getShape()->position = r2vec(640, 200);
	ranking->connect(Button::CLICKED, this, &StateMainMenu::handleRanking);
	
	instructions = new Button(new Sprite("img/instructions.png"));
	instructions->getShape()->position = r2vec(640, 300);
	instructions->connect(Button::CLICKED, this, &StateMainMenu::handleInstructions);
	
	credits = new Button(new Sprite("img/credits.png"));
	credits->getShape()->position = r2vec(640, 400);
	credits->connect(Button::CLICKED, this, &StateMainMenu::handleCredits);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(640, 500);
	quit->connect(Button::CLICKED, this, &StateMainMenu::handleQuit);
}

StateMainMenu::~StateMainMenu() {
	delete bg;
	
	delete newgame->sprite;
	delete newgame;
	
	delete ranking->sprite;
	delete ranking;
	
	delete instructions->sprite;
	delete instructions;
	
	delete credits->sprite;
	delete credits;
	
	delete quit->sprite;
	delete quit;
}

void StateMainMenu::update() {
	newgame->update();
	ranking->update();
	instructions->update();
	credits->update();
	quit->update();
}

void StateMainMenu::render() {
	bg->render();
	
	newgame->render();
	ranking->render();
	instructions->render();
	credits->render();
	quit->render();
}

void StateMainMenu::handleNewGame(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}

void StateMainMenu::handleRanking(const observer::Event& event, bool& stop) {
	throw new Change("StateRanking");
}

void StateMainMenu::handleInstructions(const observer::Event& event, bool& stop) {
	throw new Change("StateInstructions");
}

void StateMainMenu::handleCredits(const observer::Event& event, bool& stop) {
	throw new Change("StateCredits");
}
