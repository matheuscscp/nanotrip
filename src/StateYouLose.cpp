#include "StateYouLose.hpp"

#include "StateLevel.hpp"

using namespace lalge;

GAMESTATE_DEF(StateYouLose)

StateYouLose::StateYouLose(ArgsBase* args) {
	bg = new Sprite("img/level/background_youlose.png");
	
	shadow = new Sprite("img/level/shadow.png");
	eatles = new Animation("img/level/eatles_pissed.png", 0, 20, 1, 4);
	
	retry = new Button(new Sprite("img/level/button_retry.png"));
	menu = new Button(new Sprite("img/level/button_menu.png"));
	
	retry->getShape()->position = r2vec(600 - retry->sprite->srcW()/2, 572);
	retry->connect(Button::CLICKED, this, &StateYouLose::handleRetry);
	
	menu->getShape()->position = r2vec(680 + menu->sprite->srcW()/2, 572);
	menu->connect(Button::CLICKED, this, &StateYouLose::handleMenu);
}

StateYouLose::~StateYouLose() {
	delete bg;
	
	delete shadow;
	delete eatles;
	
	delete retry->sprite;
	delete retry;
	
	delete menu->sprite;
	delete menu;
}

void StateYouLose::update() {
	eatles->update();
	
	retry->update();
	menu->update();
}

void StateYouLose::render() {
	bg->render(640, 360, true);
	
	shadow->render(640, 442, true);
	eatles->render(638, 364, true);
	
	retry->render();
	menu->render();
}

void StateYouLose::handleRetry(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::TRYAGAIN_LOSE));
}

void StateYouLose::handleMenu(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::MENU));
}
