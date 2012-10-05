#include <fstream>

#include "StateLoadLevel.hpp"

#include "common.hpp"

#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateLoadLevel)

StateLoadLevel::StateLoadLevel(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/menus/background.png");
	mainmenu = new Sprite("img/menus/mainmenu.png");
	
	inputbox = new Sprite("img/menus/input_level_name.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	
	inputstring.connect(InputString::UPDATE, this, &StateLoadLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateLoadLevel::handleEnter);
	inputstring.setMaxSize(25);
	inputstring.enabled = true;
	if (args) {
		inputstring.set(((StateLevel::Args*)((StateLevel::FinalArgs*)args)->nextargs)->levelname);
		delete ((StateLevel::FinalArgs*)args)->nextargs;
		delete args;
	}
	
	warning = new Text("", "File not found!", 15, 0, SDLBase::getColor(255, 0, 0), Text::blended);
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(600 - back->sprite->rectW()/2, 600);
	back->connect(Button::CLICKED, this, &StateLoadLevel::handleGoBack);
	
	load = new Button(new Sprite("img/menus/button_load.png"));
	load->getShape()->position = r2vec(680 + load->sprite->rectW()/2, 600);
	load->connect(Button::CLICKED, this, &StateLoadLevel::handleEnter);
}

StateLoadLevel::~StateLoadLevel() {
	delete bg;
	delete mainmenu;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete back->sprite;
	delete back;
	
	delete load->sprite;
	delete load;
}

void StateLoadLevel::update() {
	inputstring.update();
	
	back->update();
	load->update();
}

void StateLoadLevel::render() {
	bg->render();
	mainmenu->render();
	
	inputbox->render(640, 425, true);
	inputtext->render(640, 440);
	if (!warning_hidden)
		warning->render(640, 465);
	
	back->render();
	load->render();
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
		throw new Change("StateLevel", new StateLevel::Args(inputstring.get(), "StateLoadLevel", new StateLevel::Args(inputstring.get(), "")));
	}
}

void StateLoadLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
