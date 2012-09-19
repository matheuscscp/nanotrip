#include "StateCredits.hpp"

using namespace lalge;

GAMESTATE_DEF(StateCredits)

StateCredits::StateCredits(ArgsBase* args) {
	bg = new Sprite("img/level/background.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateCredits::handleGoBack);
}

StateCredits::~StateCredits() {
	delete bg;
	
	delete goback->sprite;
	delete goback;
}

void StateCredits::update() {
	goback->update();
}

void StateCredits::render() {
	bg->render();
	goback->render();
}

void StateCredits::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
