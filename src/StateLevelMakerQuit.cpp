#include "StateLevelMakerQuit.hpp"

#include "SDLBase.hpp"
#include "StateLevelMaker.hpp"
#include "InputManager.hpp"

#define ENTER_DELAY	1

using namespace lalge;

GAMESTATE_DEF(StateLevelMakerQuit)

StateLevelMakerQuit::StateLevelMakerQuit(ArgsBase* args) : quit(false) {
	if (args) {
		quit = true;
		delete args;
	}
	
	bg = new Sprite("img/levelmaker/dialog_box.png");
	
	save = new Button(new Sprite("img/levelmaker/button_save.png"));
	discard = new Button(new Sprite("img/levelmaker/button_discard.png"));
	cancel = new Button(new Sprite("img/levelmaker/button_cancel.png"));
	
	save->getShape()->position = r2vec(600 - (save->sprite->srcW() + discard->sprite->srcW())/2, 395);
	save->connect(Button::CLICKED, this, &StateLevelMakerQuit::handleSave);
	
	discard->getShape()->position = r2vec(640, 395);
	discard->connect(Button::CLICKED, this, &StateLevelMakerQuit::handleDiscard);
	
	cancel->getShape()->position = r2vec(680 + (cancel->sprite->srcW() + discard->sprite->srcW())/2, 395);
	cancel->connect(Button::CLICKED, this, &StateLevelMakerQuit::handleCancel);
	
	save->selected = true;
	
	sound_error = new Audio("sfx/levelmaker/error.wav");
	sound_error->play(1);
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateLevelMakerQuit::handleKeyDown);
}

StateLevelMakerQuit::~StateLevelMakerQuit() {
	delete bg;
	
	delete save->sprite;
	delete save;
	
	delete discard->sprite;
	delete discard;
	
	delete cancel->sprite;
	delete cancel;
	
	delete sound_error;
}

void StateLevelMakerQuit::update() {
	save->update();
	discard->update();
	cancel->update();
}

void StateLevelMakerQuit::render() {
	bg->render(640, 360, true);
	
	save->render();
	discard->render();
	cancel->render();
}

void StateLevelMakerQuit::handleSave(const observer::Event& event, bool& stop) {
	if (quit)
		throw new Unstack(new StateLevelMaker::UnstackArgs(StateLevelMaker::UnstackArgs::SAVE_QUIT));
	
	throw new Unstack(new StateLevelMaker::UnstackArgs(StateLevelMaker::UnstackArgs::SAVE_MENU));
}

void StateLevelMakerQuit::handleDiscard(const observer::Event& event, bool& stop) {
	if (quit)
		throw new Unstack(new StateLevelMaker::UnstackArgs(StateLevelMaker::UnstackArgs::QUIT));
	
	throw new Unstack(new StateLevelMaker::UnstackArgs(StateLevelMaker::UnstackArgs::MENU));
}

void StateLevelMakerQuit::handleCancel(const observer::Event& event, bool& stop) {
	throw new Unstack();
}

void StateLevelMakerQuit::handleQuit(const observer::Event& event, bool& stop) {
	sound_error->play(1);
}

void StateLevelMakerQuit::handleKeyDown(const observer::Event& event, bool& stop) {
	switch (inputmanager_event.key.keysym.sym) {
	case SDLK_ESCAPE:
		throw new Unstack();
		
	case SDLK_UP:
	case SDLK_LEFT:
		save->selected = ((save->selected) ? true : discard->selected);
		discard->selected = cancel->selected;
		cancel->selected = false;
		break;
		
	case SDLK_DOWN:
	case SDLK_RIGHT:
		cancel->selected = ((cancel->selected) ? true : discard->selected);
		discard->selected = save->selected;
		save->selected = false;
		break;
		
	default:
		break;
	}
}
