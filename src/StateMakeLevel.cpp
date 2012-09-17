#include "StateMakeLevel.hpp"

#include "SDLBase.hpp"
#include "StateLevelMaker.hpp"

using namespace lalge;

GAMESTATE_DEF(StateMakeLevel)

StateMakeLevel::StateMakeLevel(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	
	inputbox = new Sprite("img/inputlevelname.png");
	
	inputtext = new Text("", "", 20, 0, SDLBase::getColor(0, 0, 0), Text::shaded, SDLBase::getColor(255, 255, 255));
	
	inputstring.connect(InputString::UPDATE, this, &StateMakeLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateMakeLevel::handleEnter);
	inputstring.setMaxSize(60);
	
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
}

void StateMakeLevel::handleInput(const observer::Event& event, bool& stop) {
	inputtext->setText(inputstring.get());
}

void StateMakeLevel::handleEnter(const observer::Event& event, bool& stop) {
	throw new Change("StateLevelMaker", new StateLevelMaker::Args(inputstring.get()));
}

void StateMakeLevel::handleGoBack(const observer::Event& event, bool& stop) {
	throw new Change("StateNewGame");
}
