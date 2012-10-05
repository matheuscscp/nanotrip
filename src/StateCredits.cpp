#include "StateCredits.hpp"

using namespace lalge;

GAMESTATE_DEF(StateCredits)

StateCredits::StateCredits(ArgsBase* args) {
	bg = new Sprite("img/menus/background.png");
	credits = new Sprite("img/menus/credits.png");
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(640, 600);
	back->connect(Button::CLICKED, this, &StateCredits::handleGoBack);
}

StateCredits::~StateCredits() {
	delete bg;
	delete credits;
	
	delete back->sprite;
	delete back;
}

void StateCredits::update() {
	back->update();
}

void StateCredits::render() {
	bg->render();
	credits->render();
	
	back->render();
}

void StateCredits::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
