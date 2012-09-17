#include <fstream>

#include "StateMakeLevel.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevelMaker.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateMakeLevel)

StateMakeLevel::StateMakeLevel(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/bg_opaco.png");
	
	inputbox = new Sprite("img/inputlevelname.png");
	
	inputtext = new Text("", "", 20, 0, SDLBase::getColor(0, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
	inputstring.connect(InputString::UPDATE, this, &StateMakeLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateMakeLevel::handleEnter);
	inputstring.setMaxSize(60);
	
	warning = new Text("", "This level already exists!", 15, 0, SDLBase::getColor(255, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(540, 500);
	goback->connect(Button::CLICKED, this, &StateMakeLevel::handleGoBack);
	
	quit = new Button(new Sprite("img/quit.png"));
	quit->getShape()->position = r2vec(740, 500);
	quit->connect(Button::CLICKED, this, &StateMakeLevel::handleQuit);
}

void StateMakeLevel::update() {
	inputstring.update();
}

void StateMakeLevel::render() {
	inputbox->render(640, 300, true);
	inputtext->render(640, 320);
	if (!warning_hidden)
		warning->render(640, 345);
}

void StateMakeLevel::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
	warning_hidden = true;
}

void StateMakeLevel::handleEnter(const observer::Event& event, bool& stop) {
	std::fstream f(RootPath::get("level/" + inputstring.get() + ".conf").c_str());
	if (f.is_open())
		warning_hidden = false;
	else {
		f.close();
		throw new Change("StateLevelMaker", new StateLevelMaker::Args(inputstring.get()));
	}
}

void StateMakeLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
