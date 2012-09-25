#include "StatePause.hpp"

#include "InputManager.hpp"
#include "SDLBase.hpp"
#include "StateLevel.hpp"

using namespace lalge;

GAMESTATE_DEF(StatePause)

StatePause::StatePause(ArgsBase* args) : retry(0) {
	bg = new Sprite("img/pause/background.png");
	
	pause = new Text("ttf/Swiss721BlackRoundedBT.ttf", "PAUSE", 100, 0, SDLBase::getColor(255, 255, 255), Text::blended);
	
	resume = new Button(new Sprite("img/pause/resume.png"));
	resume->getShape()->position = r2vec(640, 380);
	resume->connect(Button::CLICKED, this, &StatePause::handleResume);
	
	if (args)
		delete args;
	else {
		retry = new Button(new Sprite("img/pause/retry.png"));
		retry->getShape()->position = r2vec(640, 450);
		retry->connect(Button::CLICKED, this, &StatePause::handleRetry);
	}
	
	quitlevel = new Button(new Sprite("img/pause/quitlevel.png"));
	quitlevel->getShape()->position = r2vec(640, 520);
	quitlevel->connect(Button::CLICKED, this, &StatePause::handleQuitLevel);
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StatePause::handleKeyDown);
}

StatePause::~StatePause() {
	delete bg;
	
	delete pause;
	
	delete resume->sprite;
	delete resume;
	
	if (retry) {
		delete retry->sprite;
		delete retry;
	}
	
	delete quitlevel->sprite;
	delete quitlevel;
}

void StatePause::update() {
	resume->update();
	if (retry)
		retry->update();
	quitlevel->update();
}

void StatePause::render() {
	bg->render(640, 360, true);
	pause->render(640, 260);
	resume->render();
	if (retry)
		retry->render();
	quitlevel->render();
}

void StatePause::handleKeyDown(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_ESCAPE)
		throw new Unstack();
}

void StatePause::handleResume(const observer::Event& event, bool& stop) {
	throw new Unstack();
}

void StatePause::handleRetry(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::RETRY));
}

void StatePause::handleQuitLevel(const observer::Event& event, bool& stop) {
	throw new Unstack(new StateLevel::UnstackArgs(StateLevel::UnstackArgs::QUITLEVEL));
}
