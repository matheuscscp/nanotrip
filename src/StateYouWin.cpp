#include <sstream>

#include "StateYouWin.hpp"

#include "StateLevel.hpp"
#include "SDLBase.hpp"

using namespace lalge;

GAMESTATE_DEF(StateYouWin)

StateYouWin::StateYouWin(ArgsBase* args) {
	bg = new Sprite("img/youwin/background.png");
	
	text_you_win = new Text("ttf/Swiss721BlackRoundedBT.ttf", "YOU WIN", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	std::stringstream ss;
	ss << "Score: ";
	ss << ((StateLevel::FinalArgs*)args)->points;
	delete args;
	text_points = new Text("ttf/Swiss721BlackRoundedBT.ttf", ss.str(), 40, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	
	tryagain = new Button(new Sprite("img/youwin/tryagain.png"));
	tryagain->getShape()->position = r2vec(640, 510);
	tryagain->connect(Button::CLICKED, this, &StateYouWin::handleTryAgain);
	
	goahead = new Button(new Sprite("img/youwin/goahead.png"));
	goahead->getShape()->position = r2vec(640, 560);
	goahead->connect(Button::CLICKED, this, &StateYouWin::handleGoAhead);
}

StateYouWin::~StateYouWin() {
	delete bg;
	
	delete text_you_win;
	delete text_points;
	
	delete tryagain->sprite;
	delete tryagain;
	
	delete goahead->sprite;
	delete goahead;
}

void StateYouWin::update() {
	tryagain->update();
	goahead->update();
}

void StateYouWin::render() {
	bg->render(640, 360, true);
	
	text_you_win->render(640, 210);
	text_points->render(640, 330);
	
	tryagain->render();
	goahead->render();
}

void StateYouWin::handleTryAgain(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::TRYAGAIN));
}

void StateYouWin::handleGoAhead(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::GOAHEAD));
}
