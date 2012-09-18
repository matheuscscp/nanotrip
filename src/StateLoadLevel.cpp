#include <fstream>

#include "StateLoadLevel.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateLoadLevel)

StateLoadLevel::StateLoadLevel(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/bg_opaco.png");
	
	inputbox = new Sprite("img/inputlevelname.png");
	
	inputtext = new Text("", "", 20, 0, SDLBase::getColor(0, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
	inputstring.connect(InputString::UPDATE, this, &StateLoadLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateLoadLevel::handleEnter);
	inputstring.setMaxSize(60);
	
	warning = new Text("", "File not found!", 15, 0, SDLBase::getColor(255, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(540, 500);
	goback->connect(Button::CLICKED, this, &StateLoadLevel::handleGoBack);
	
	play = new Button(new Sprite("img/play.png"));
	play->getShape()->position = r2vec(740, 500);
	play->connect(Button::CLICKED, this, &StateLoadLevel::handleEnter);
}

void StateLoadLevel::update() {
	inputstring.update();
}

void StateLoadLevel::render() {
	GameObject::renderAll();
	inputbox->render(640, 300, true);
	inputtext->render(640, 320);
	if (!warning_hidden)
		warning->render(640, 345);
}

void StateLoadLevel::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
	warning_hidden = true;
}

void StateLoadLevel::handleEnter(const observer::Event& event, bool& stop) {
	std::fstream f(RootPath::get("level/" + inputstring.get() + ".conf").c_str());
	if (!f.is_open())
		warning_hidden = false;
	else {
		f.close();
		throw new Change("StateLevel", new StateLevel::Args(inputstring.get()));
	}
}

void StateLoadLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
