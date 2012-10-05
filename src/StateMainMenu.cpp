#include "StateMainMenu.hpp"

#include "common.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateMainMenu)

StateMainMenu::StateMainMenu(ArgsBase* args) {
	bg = new Sprite("img/menus/background.png");
	mainmenu = new Sprite("img/menus/mainmenu.png");
	
	newgame = new Button(new Sprite("img/menus/button_new_game.png"));
	newgame->getShape()->position = r2vec(640, 335);
	newgame->connect(Button::CLICKED, this, &StateMainMenu::handleNewGame);
	
	ranking = new Button(new Sprite("img/menus/button_ranking.png"));
	ranking->getShape()->position = r2vec(640, 385);
	ranking->connect(Button::CLICKED, this, &StateMainMenu::handleRanking);
	
	instructions = new Button(new Sprite("img/menus/button_instructions.png"));
	instructions->getShape()->position = r2vec(640, 435);
	instructions->connect(Button::CLICKED, this, &StateMainMenu::handleInstructions);
	
	credits = new Button(new Sprite("img/menus/button_credits.png"));
	credits->getShape()->position = r2vec(640, 485);
	credits->connect(Button::CLICKED, this, &StateMainMenu::handleCredits);
	
	quit = new Button(new Sprite("img/menus/button_quit.png"));
	quit->getShape()->position = r2vec(640, 535);
	quit->connect(Button::CLICKED, this, &StateMainMenu::handleQuit);
}

StateMainMenu::~StateMainMenu() {
	delete bg;
	delete mainmenu;
	
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
	mainmenu->render();
	
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
