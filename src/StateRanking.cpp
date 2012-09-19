#include "StateRanking.hpp"

using namespace lalge;

GAMESTATE_DEF(StateRanking)

StateRanking::StateRanking(ArgsBase* args) {
	bg = new Sprite("img/level/background.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateRanking::handleGoBack);
}

StateRanking::~StateRanking() {
	delete bg;
	
	delete goback->sprite;
	delete goback;
}

void StateRanking::update() {
	goback->update();
}

void StateRanking::render() {
	bg->render();
	goback->render();
}

void StateRanking::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
