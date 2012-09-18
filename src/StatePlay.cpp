#include <fstream>

#include "StatePlay.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StatePlay)

StatePlay::StatePlay(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	inputbox = new Sprite("img/inputname.png");
	
	inputtext = new Text("", "", 20, 0, SDLBase::getColor(0, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
	inputstring.connect(InputString::UPDATE, this, &StatePlay::handleInput);
	inputstring.connect(InputString::ENTER, this, &StatePlay::handleEnter);
	inputstring.setMaxSize(60);
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(540, 500);
	goback->connect(Button::CLICKED, this, &StatePlay::handleGoBack);
	
	play = new Button(new Sprite("img/play.png"));
	play->getShape()->position = r2vec(740, 500);
	play->connect(Button::CLICKED, this, &StatePlay::handleEnter);
}

void StatePlay::update() {
	inputstring.update();
}

void StatePlay::render() {
	GameObject::renderAll();
	inputbox->render(640, 300, true);
	inputtext->render(640, 320);
}

void StatePlay::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
}

void StatePlay::handleEnter(const observer::Event& event, bool& stop) {
	SHOW("handleEnter");
}

void StatePlay::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
