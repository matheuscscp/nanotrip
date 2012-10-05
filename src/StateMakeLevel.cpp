#include "StateMakeLevel.hpp"

#include "SDLBase.hpp"
#include "StateLevelMaker.hpp"

using namespace lalge;

GAMESTATE_DEF(StateMakeLevel)

StateMakeLevel::StateMakeLevel(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/menus/background.png");
	mainmenu = new Sprite("img/menus/mainmenu.png");
	
	inputbox = new Sprite("img/menus/input_level_name.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	
	inputstring.connect(InputString::UPDATE, this, &StateMakeLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateMakeLevel::handleEnter);
	inputstring.setMaxSize(25);
	inputstring.enabled = true;
	if (args) {
		inputstring.set(((StateLevelMaker::Args*)args)->levelname);
		delete args;
	}
	
	warning = new Text("", "Empty name!", 15, 0, SDLBase::getColor(255, 0, 0), Text::blended);
	
	back = new Button(new Sprite("img/menus/button_back.png"));
	back->getShape()->position = r2vec(600 - back->sprite->rectW()/2, 600);
	back->connect(Button::CLICKED, this, &StateMakeLevel::handleGoBack);
	
	make = new Button(new Sprite("img/menus/button_make.png"));
	make->getShape()->position = r2vec(680 + make->sprite->rectW()/2, 600);
	make->connect(Button::CLICKED, this, &StateMakeLevel::handleEnter);
}

StateMakeLevel::~StateMakeLevel() {
	delete bg;
	delete mainmenu;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete back->sprite;
	delete back;
	
	delete make->sprite;
	delete make;
}

void StateMakeLevel::update() {
	inputstring.update();
	
	back->update();
	make->update();
}

void StateMakeLevel::render() {
	bg->render();
	mainmenu->render();
	
	inputbox->render(640, 425, true);
	inputtext->render(640, 440);
	if (!warning_hidden)
		warning->render(640, 465);
	
	back->render();
	make->render();
}

void StateMakeLevel::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
	warning_hidden = true;
}

void StateMakeLevel::handleEnter(const observer::Event& event, bool& stop) {
	if (inputstring.get().size())
		throw new Change("StateLevelMaker", new StateLevelMaker::Args(inputstring.get()));
	
	warning_hidden = false;
}

void StateMakeLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
