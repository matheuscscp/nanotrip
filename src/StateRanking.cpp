#include "StateRanking.hpp"

#include "SDLBase.hpp"

using namespace lalge;

GAMESTATE_DEF(StateRanking)

StateRanking::StateRanking(ArgsBase* args) {
	bg = new Sprite("img/menus/background.png");
	ranking = new Sprite("img/menus/ranking.png");
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(640, 600);
	back->connect(Button::CLICKED, this, &StateRanking::handleGoBack);
}

StateRanking::~StateRanking() {
	delete bg;
	delete ranking;
	
	delete back->sprite;
	delete back;
}

void StateRanking::update() {
	back->update();
}

void StateRanking::render() {
	bg->render();
	ranking->render();
	
	ranking_data.render(640, 390);
	
	back->render();
}

void StateRanking::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
