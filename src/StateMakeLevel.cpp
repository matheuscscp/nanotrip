#include "StateMakeLevel.hpp"

#include "SDLBase.hpp"
#include "StateLevelMaker.hpp"

using namespace lalge;

GAMESTATE_DEF(StateMakeLevel)

StateMakeLevel::StateMakeLevel(ArgsBase* args) : warning_hidden(true) {
	bg = new Sprite("img/level/background.png");
	
	inputbox = new Sprite("img/menus/inputlevelname.png");
	
	inputtext = new Text("ttf/Swiss721BlackRoundedBT.ttf", "", 20, 0, SDLBase::getColor(51, 51, 51), Text::blended);
	
	inputstring.connect(InputString::UPDATE, this, &StateMakeLevel::handleInput);
	inputstring.connect(InputString::ENTER, this, &StateMakeLevel::handleEnter);
	inputstring.setMaxSize(30);
	if (args) {
		inputstring.set(((StateLevelMaker::Args*)args)->levelname);
		delete args;
	}
	
	warning = new Text("", "Empty name!", 15, 0, SDLBase::getColor(255, 0, 0), Text::blended);
	
	goback = new Button(new Sprite("img/goback.png"));
	goback->getShape()->position = r2vec(540, 500);
	goback->connect(Button::CLICKED, this, &StateMakeLevel::handleGoBack);
	
	makelevel = new Button(new Sprite("img/makelevel.png"));
	makelevel->getShape()->position = r2vec(740, 500);
	makelevel->connect(Button::CLICKED, this, &StateMakeLevel::handleEnter);
}

StateMakeLevel::~StateMakeLevel() {
	delete bg;
	
	delete inputbox;
	delete inputtext;
	delete warning;
	
	delete goback->sprite;
	delete goback;
	
	delete makelevel->sprite;
	delete makelevel;
}

void StateMakeLevel::update() {
	inputstring.update();
	
	goback->update();
	makelevel->update();
}

void StateMakeLevel::render() {
	bg->render();
	
	inputbox->render(640, 300, true);
	inputtext->render(640, 315);
	if (!warning_hidden)
		warning->render(640, 340);
	
	goback->render();
	makelevel->render();
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
