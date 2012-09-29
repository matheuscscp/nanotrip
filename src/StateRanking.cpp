#include "StateRanking.hpp"

using namespace lalge;

GAMESTATE_DEF(StateRanking)

StateRanking::StateRanking(ArgsBase* args) {
	bg = new Sprite("img/level/background.png");
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(100, 680);
	back->connect(Button::CLICKED, this, &StateRanking::handleGoBack);
}

StateRanking::~StateRanking() {
	delete bg;
	
	delete back->sprite;
	delete back;
}

void StateRanking::update() {
	back->update();
}

void StateRanking::render() {
	bg->render();
	back->render();
}

void StateRanking::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
