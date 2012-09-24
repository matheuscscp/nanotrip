#include "StatePause.hpp"

#include "InputManager.hpp"
#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;

GAMESTATE_DEF(StatePause)

StatePause::StatePause(ArgsBase* args) : restart(0) {
	bg = new Sprite("img/pause/background.png");
	
	pause = new Text("ttf/Swiss721BlackRoundedBT.ttf", "PAUSE", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	
	resume = new Button(new Sprite("img/pause/resume.png"));
	resume->getShape()->position = r2vec(640, 380);
	resume->connect(Button::CLICKED, this, &StatePause::handleResume);
	
	if (args)
		delete args;
	else {
		restart = new Button(new Sprite("img/pause/restart.png"));
		restart->getShape()->position = r2vec(640, 450);
		restart->connect(Button::CLICKED, this, &StatePause::handleRestart);
	}
	
	menu = new Button(new Sprite("img/pause/menu.png"));
	menu->getShape()->position = r2vec(640, 520);
	menu->connect(Button::CLICKED, this, &StatePause::handleMenu);
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StatePause::handleKeyDown);
}

StatePause::~StatePause() {
	delete bg;
	
	delete pause;
	
	delete resume->sprite;
	delete resume;
	
	if (restart) {
		delete restart->sprite;
		delete restart;
	}
	
	delete menu->sprite;
	delete menu;
}

void StatePause::update() {
	resume->update();
	if (restart)
		restart->update();
	menu->update();
}

void StatePause::render() {
	bg->render(640, 360, true);
	pause->render(640, 260);
	resume->render();
	if (restart)
		restart->render();
	menu->render();
}

void StatePause::handleKeyDown(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_ESCAPE)
		throw new Unstack();
}

void StatePause::handleResume(const observer::Event& event, bool& stop) {
	throw new Unstack();
}

void StatePause::handleRestart(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::RESTART));
}

void StatePause::handleMenu(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::MENU));
}
