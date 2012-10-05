#include <fstream>

#include "StatePlay.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevel.hpp"
#include "StateTransition.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StatePlay)

StatePlay::StatePlay(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/menus/background.png");
	mainmenu = new Sprite("img/menus/mainmenu.png");
	
	inputbox = new Sprite("img/menus/input_name.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	
	inputstring.connect(InputString::UPDATE, this, &StatePlay::handleInput);
	inputstring.connect(InputString::ENTER, this, &StatePlay::handleEnter);
	inputstring.setMaxSize(25);
	inputstring.enabled = true;
	if (args) {
		inputstring.set(((StateTransition::Args*)args)->player_name);
		delete args;
	}
	
	warning = new Text("", "Empty name!", 15, 0, SDLBase::getColor(255, 0, 0), Text::blended);
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(600 - back->sprite->rectW()/2, 600);
	back->connect(Button::CLICKED, this, &StatePlay::handleGoBack);
	
	play = new Button(new Sprite("img/menus/button_play.png"));
	play->getShape()->position = r2vec(680 + play->sprite->rectW()/2, 600);
	play->connect(Button::CLICKED, this, &StatePlay::handleEnter);
}

StatePlay::~StatePlay() {
	delete bg;
	delete mainmenu;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete back->sprite;
	delete back;
	
	delete play->sprite;
	delete play;
}

void StatePlay::update() {
	inputstring.update();
	
	back->update();
	play->update();
}

void StatePlay::render() {
	bg->render();
	mainmenu->render();
	
	inputbox->render(640, 425, true);
	inputtext->render(640, 440);
	if (!warning_hidden)
		warning->render(640, 465);
	
	back->render();
	play->render();
}

void StatePlay::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
	warning_hidden = true;
}

void StatePlay::handleEnter(const observer::Event& event, bool& stop) {
	if (inputstring.get().size())
		throw new Change("StateTransition", new StateTransition::Args(inputstring.get()));
	
	warning_hidden = false;
}

void StatePlay::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
