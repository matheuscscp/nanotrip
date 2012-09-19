#include "StateLevelMaker.hpp"

#include "InputManager.hpp"

GAMESTATE_DEF(StateLevelMaker)

StateLevelMaker::Args::Args(const std::string& levelname) : levelname(levelname) {}

StateLevelMaker::StateLevelMaker(ArgsBase* args) :
levelname(((Args*)args)->levelname), state(1) {
	delete args;
	
	// only one opaque background renderization
	bg = new Sprite("img/level/background.png");
	bg->render();
	bg->setAlpha(0.3f);
	
	hud = new Sprite("img/level/hud.png");
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevelMaker::handleKeyDown);
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &StateLevelMaker::handleMouseDownLeft);
	
	
}

void StateLevelMaker::handleUnstack(ArgsBase* args) {
	frozen_ = false;
}

void StateLevelMaker::update() {
}

void StateLevelMaker::render() {
	//GameObject::renderAll();
	hud->render();
}

void StateLevelMaker::handleKeyDown(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_ESCAPE) {
		//frozen_ = true;
		//throw new StackUp("StateLevelMakerPause");
	}
}

void StateLevelMaker::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	//TODO
}
