#include <sstream>

#include "StateYouWin.hpp"

#include "StateLevel.hpp"
#include "SDLBase.hpp"
#include "Animation.hpp"

using namespace lalge;

GAMESTATE_DEF(StateYouWin)

StateYouWin::StateYouWin(ArgsBase* args) {
	bg = new Sprite("img/youwin/background.png");
	
	shadow = new Sprite("img/youwin/shadow.png");
	eatles = new Animation("img/youwin/eatles.png", 0, 8, 1, 4);
	
	std::stringstream ss;
	ss << ((StateLevel::FinalArgs*)args)->points;
	delete args;
	
	text_score = new Text("ttf/Swiss721BlackRoundedBT.ttf", "score:", 22, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	text_points = new Text("ttf/Swiss721BlackRoundedBT.ttf", ss.str(), 50, 0, SDLBase::getColor(63, 217, 255), Text::blended);
	
	retry = new Button(new Sprite("img/youwin/retry.png"));
	menu = new Button(new Sprite("img/youwin/menu.png"));
	next = new Button(new Sprite("img/youwin/next.png"));
	
	retry->getShape()->position = r2vec(600 - (retry->sprite->srcW() + menu->sprite->srcW())/2, 572);
	retry->connect(Button::CLICKED, this, &StateYouWin::handleRetry);
	
	menu->getShape()->position = r2vec(640, 572);
	menu->connect(Button::CLICKED, this, &StateYouWin::handleMenu);
	
	next->getShape()->position = r2vec(680 + (next->sprite->srcW() + menu->sprite->srcW())/2, 572);
	next->connect(Button::CLICKED, this, &StateYouWin::handleNext);
}

StateYouWin::~StateYouWin() {
	delete bg;
	
	delete shadow;
	delete eatles;
	
	delete text_score;
	delete text_points;
	
	delete retry->sprite;
	delete retry;
	
	delete menu->sprite;
	delete menu;
	
	delete next->sprite;
	delete next;
}

void StateYouWin::update() {
	eatles->update();
	
	retry->update();
	menu->update();
	next->update();
}

void StateYouWin::render() {
	bg->render(640, 360, true);
	
	shadow->render(640, 392, true);
	eatles->render(638, 314, true);
	
	text_score->render(640, 450);
	text_points->render(640, 450 + (text_score->h() + text_points->h())/2);
	
	retry->render();
	menu->render();
	next->render();
}

void StateYouWin::handleRetry(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::TRYAGAIN_WIN));
}

void StateYouWin::handleMenu(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::MENU));
}

void StateYouWin::handleNext(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::NEXT));
}
