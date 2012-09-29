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
	
	inputbox = new Sprite("img/menus/input_level_name.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	
	inputstring.connect(InputString::UPDATE, this, &StateLoadLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateLoadLevel::handleEnter);
	inputstring.setMaxSize(25);
	
	warning = new Text("", "File not found!", 15, 0, SDLBase::getColor(255, 0, 0), Text::blended);
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(540, 500);
	back->connect(Button::CLICKED, this, &StateLoadLevel::handleGoBack);
	
	play = new Button(new Sprite("img/menus/button_play.png"));
	play->getShape()->position = r2vec(740, 500);
	play->connect(Button::CLICKED, this, &StateLoadLevel::handleEnter);
}

StateLoadLevel::~StateLoadLevel() {
	delete bg;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete back->sprite;
	delete back;
	
	delete play->sprite;
	delete play;
}

void StateLoadLevel::update() {
	inputstring.update();
	
	back->update();
	play->update();
}

void StateLoadLevel::render() {
	bg->render();
	
	inputbox->render(640, 300, true);
	inputtext->render(640, 315);
	if (!warning_hidden)
		warning->render(640, 340);
	
	back->render();
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
