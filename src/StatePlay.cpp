#include <fstream>

#include "StatePlay.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StatePlay)

StatePlay::StatePlay(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/level/background.png");
	
	inputbox = new Sprite("img/menus/inputname.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	
	inputstring.connect(InputString::UPDATE, this, &StatePlay::handleInput);
	inputstring.connect(InputString::ENTER, this, &StatePlay::handleEnter);
	inputstring.setMaxSize(30);
	
	warning = new Text("", "Empty name!", 15, 0, SDLBase::getColor(255, 0, 0), Text::blended);
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(540, 500);
	goback->connect(Button::CLICKED, this, &StatePlay::handleGoBack);
	
	play = new Button(new Sprite("img/play.png"));
	play->getShape()->position = r2vec(740, 500);
	play->connect(Button::CLICKED, this, &StatePlay::handleEnter);
}

StatePlay::~StatePlay() {
	delete bg;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete goback->sprite;
	delete goback;
	
	delete play->sprite;
	delete play;
}

void StatePlay::update() {
	inputstring.update();
	
	goback->update();
	play->update();
}

void StatePlay::render() {
	bg->render();
	
	inputbox->render(640, 300, true);
	inputtext->render(640, 315);
	if (!warning_hidden)
		warning->render(640, 340);
	
	goback->render();
	play->render();
}

void StatePlay::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
	warning_hidden = true;
}

void StatePlay::handleEnter(const observer::Event& event, bool& stop) {
	if (inputstring.get().size()) {
		SHOW("handleEnter");
		return;	//TODO trocar pelo throw do primeiro level da historia
	}
	
	warning_hidden = false;
}

void StatePlay::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
