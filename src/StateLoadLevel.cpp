#include <fstream>

#include "StateLoadLevel.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateLoadLevel)

StateLoadLevel::StateLoadLevel(ArgsBase* args) : warning_hidden(true) {
	if (args) {
		if (((StateLevel::FinalArgs*)args)->nextargs)
			delete ((StateLevel::FinalArgs*)args)->nextargs;
		delete args;
	}
	
	bg = new Sprite("img/level/background.png");
	
	inputbox = new Sprite("img/inputlevelname.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 18, 0, SDLBase::getColor(0, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
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

StateLoadLevel::~StateLoadLevel() {
	delete bg;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete goback->sprite;
	delete goback;
	
	delete play->sprite;
	delete play;
}

void StateLoadLevel::update() {
	inputstring.update();
	
	goback->update();
	play->update();
}

void StateLoadLevel::render() {
	bg->render();
	
	inputbox->render(640, 300, true);
	inputtext->render(640, 320);
	if (!warning_hidden)
		warning->render(640, 345);
	
	goback->render();
	play->render();
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
		throw new Change("StateLevel", new StateLevel::Args(inputstring.get(), "StateLoadLevel"));
	}
}

void StateLoadLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
