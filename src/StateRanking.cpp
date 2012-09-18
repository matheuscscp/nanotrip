#include "StateRanking.hpp"

using namespace lalge;

GAMESTATE_DEF(StateRanking)

StateRanking::StateRanking(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(100, 680);
	goback->connect(Button::CLICKED, this, &StateRanking::handleGoBack);
}

void StateRanking::render() {
	GameObject::renderAll();
}

void StateRanking::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
