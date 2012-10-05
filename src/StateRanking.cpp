#include "StateRanking.hpp"

#include "SDLBase.hpp"

using namespace lalge;

GAMESTATE_DEF(StateRanking)

StateRanking::StateRanking(ArgsBase* args) {
	bg = new Sprite("img/menus/background.png");
	ranking = new Sprite("img/menus/ranking.png");
	
	text = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 13, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	ranking_data = new Ranking("bin/nanotrip.rnk", text, 30000);
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(640, 600);
	back->connect(Button::CLICKED, this, &StateRanking::handleGoBack);
}

StateRanking::~StateRanking() {
	delete bg;
	delete ranking;
	
	delete text;
	delete ranking_data;
	
	delete back->sprite;
	delete back;
}

void StateRanking::update() {
	back->update();
}

void StateRanking::render() {
	bg->render();
	ranking->render();
	
	ranking_data->render();
	
	back->render();
}

void StateRanking::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateMainMenu");
}
