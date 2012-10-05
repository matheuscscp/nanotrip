#include "StateCredits.hpp"

using namespace lalge;

GAMESTATE_DEF(StateCredits)

StateCredits::StateCredits(ArgsBase* args) : show_ranking(false) {
	bg = new Sprite("img/menus/background.png");
	credits = new Sprite("img/menus/credits.png");
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(640, 600);
	back->connect(Button::CLICKED, this, &StateCredits::handleGoBack);
	
	if (args) {
		delete args;
		show_ranking = true;
	}
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
	if (show_ranking)
		throw new Change("StateRanking");
	
	throw new Change("StateMainMenu");
}
