#include "StateGameOver.hpp"

#include "StateLevel.hpp"
#include "InputManager.hpp"

using namespace lalge;

GAMESTATE_DEF(StateGameOver)

StateGameOver::StateGameOver(ArgsBase* args) {
	bg = new Sprite("img/gameover/background.png");
	
	tryagain = new Button(new Sprite("img/gameover/tryagain.png"));
	tryagain->getShape()->position = r2vec(280, 650);
	tryagain->connect(Button::CLICKED, this, &StateGameOver::handleTryAgain);
	
	mainmenu = new Button(new Sprite("img/gameover/mainmenu.png"));
	mainmenu->getShape()->position = r2vec(1000, 650);
	mainmenu->connect(Button::CLICKED, this, &StateGameOver::handleMainMenu);
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateGameOver::handleKeyDown);
}

StateGameOver::~StateGameOver() {
	delete bg;
	
	delete tryagain->sprite;
	delete tryagain;
	
	delete mainmenu->sprite;
	delete mainmenu;
}

void StateGameOver::update() {
	tryagain->update();
	mainmenu->update();
}

void StateGameOver::render() {
	bg->render();
	tryagain->render();
	mainmenu->render();
}

void StateGameOver::handleKeyDown(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_ESCAPE)
		throw new Unstack();
}

void StateGameOver::handleTryAgain(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::TRYAGAIN));
}

void StateGameOver::handleMainMenu(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::MAINMENU));
}
