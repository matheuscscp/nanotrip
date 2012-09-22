#include "StateMenu.hpp"

#include "InputManager.hpp"

using namespace lalge;

GAMESTATE_DEF(StateMenu)

StateMenu::StateMenu(ArgsBase* args) {
	bg = new Sprite("img/level/menu.png");
	
	resume = new Button(new Sprite("img/goback.png"));
	resume->getShape()->position = r2vec(640, 330);
	resume->connect(Button::CLICKED, this, &StateMenu::handleResume);
	
	mainmenu = new Button(new Sprite("img/goback.png"));
	mainmenu->getShape()->position = r2vec(640, 390);
	mainmenu->connect(Button::CLICKED, this, &StateMenu::handleMainMenu);
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateMenu::handleKeyDown);
}

StateMenu::~StateMenu() {
	delete bg;
	
	delete resume->sprite;
	delete resume;
	
	delete mainmenu->sprite;
	delete mainmenu;
}

void StateMenu::update() {
	resume->update();
	mainmenu->update();
}

void StateMenu::render() {
	bg->render(640, 360, true);
	resume->render();
	mainmenu->render();
}

void StateMenu::handleKeyDown(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_ESCAPE)
		throw new Unstack();
}

void StateMenu::handleResume(const observer::Event& event, bool& stop) {
	throw new Unstack();
}

void StateMenu::handleMainMenu(const observer::Event& event, bool& stop) {
	throw new Unstack(new ArgsBase());
}
