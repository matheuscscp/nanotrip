#include "StateIntroduction.hpp"

#include "SDLBase.hpp"
#include "GameBGM.hpp"
#include "InputManager.hpp"

#define BEGIN_FADEIN_ALPHA_DIFF	-0.01

#define BEGIN_DELAY	4000
#define TRUCK_DELAY	2500
#define PRESS_DELAY	6000

// eatles delays
#define EATLES_WALK		1000
#define EATLES_GETUP	500

// fade in delays
#define FADEIN_START	4000
#define FADEIN_OVER		5000

#define MENU_FADEIN_ALPHA_DIFF	-0.05

using namespace lalge;

GAMESTATE_DEF(StateIntroduction)

StateIntroduction::StateIntroduction(ArgsBase* args) :
pressed_enter(false),
played_sound(false)
{
	GameBGM::stop();
	
	storyboard = new Sprite("img/introduction/storyboard.png");
	truck = new Sprite("img/introduction/truck.png");
	press = new Animation("img/introduction/press_enter.png", 0, 12, 1, 10);
	mainmenu = new Sprite("img/introduction/mainmenu.png");
	bg = new Sprite("img/introduction/background.png");
	
	eatles_states[0] = new Animation("img/introduction/eatles_busted.png", 0, 10, 1, 4);
	eatles_states[1] = new Animation("img/introduction/eatles_smoke.png", 0, 10, 1, 8);
	eatles_states[2] = new Animation("img/introduction/eatles_walk.png", 0, 10, 1, 4);
	
	eatles = eatles_states[0];
	
	storyboard_position = r2vec(0, -1);
	storyboard_speed = r2vec(0, -100);
	
	truck_position = r2vec(490, 140);
	truck_speed = r2vec(600, 200);
	
	eatles_position = r2vec(220, 510);
	eatles_speed = r2vec(300, 0);
	
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &StateIntroduction::handleKeyDown);
	
	fadein_alpha = 1;
	SDLBase::setFadeAlpha(fadein_alpha);
	
	stopwatch_begin.start();
	
	// sounds
	sound_scream = new Audio("sfx/introduction/scream.wav");
	sound_bird = new Audio("sfx/introduction/bird.wav");
	sound_fall = new Audio("sfx/introduction/fall.wav");
	sound_truck = new Audio("sfx/introduction/truck.wav");
	sound_eatles_getup = new Audio("sfx/introduction/eatles_getup.wav");
	
	Audio::musicVolume(0.5);
	GameBGM::play();
}

StateIntroduction::~StateIntroduction() {
	Audio::musicVolume(1);
	
	delete storyboard;
	delete truck;
	delete press;
	delete mainmenu;
	delete bg;
	
	for (int i = 0; i < 3; ++i)
		delete eatles_states[i];
	
	delete sound_scream;
	delete sound_bird;
	delete sound_fall;
	delete sound_truck;
	delete sound_eatles_getup;
}

void StateIntroduction::update() {
	// fade in
	if (stopwatch_begin.time() <= BEGIN_DELAY) {
		fadein_alpha += BEGIN_FADEIN_ALPHA_DIFF;
		if (fadein_alpha < 0)
			fadein_alpha = 0;
		SDLBase::setFadeAlpha(fadein_alpha);
		return;
	}
	
	if (stopwatch_fadein.time() > FADEIN_OVER)
		throw new Change("StateMainMenu");
	
	press->update();
	
	updateEatles();
	
	Scalar dt = float(SDLBase::dt())/1000;
	Scalar y = storyboard_position.x(1);
	
	if (stopwatch_end.time() > PRESS_DELAY)	// waiting player to press enter
		return;
	else if (stopwatch_end.time() > TRUCK_DELAY) {	// truck update
		truck_position += truck_speed*dt;
		return;
	}
	else if (stopwatch_end.time() >= 0)	// waiting truck to go away
		return;
	else if (y < -5900) {	// almost hitting the ground
		if (!played_sound) {
			sound_fall->play(1);
			played_sound = true;
		}
	}
	else if (y < -4904) {	// after bird disappear
		storyboard_speed.set(1, -900);
		played_sound = false;
	}
	else if (y < -4350) {	// right after scream sound
		if (!played_sound) {
			sound_bird->play(1);
			played_sound = true;
		}
	}
	else if (y < -4184)	{// after bird appear
		storyboard_speed.set(1, -150);
		played_sound = false;
	}
	else if (y < -3900) {	// almost bird appearing
		if (!played_sound) {
			sound_scream->play(1);
			played_sound = true;
		}
	}
	else if (y < -1184)	// after star appear
		storyboard_acceleration.set(1, 0);
	else if (y < -760)	// after moon disappear
		storyboard_acceleration.set(1, -2500);
	
	// storyboard update
	{
		R2Vector tmp_pos = storyboard_position + storyboard_speed*dt + storyboard_acceleration*(dt*dt/2);
		
		if (tmp_pos.x(1) > -6480)
			storyboard_position = tmp_pos;
		else {
			storyboard_position.set(1, -6480);
			stopwatch_end.start();
			sound_truck->play(1);
		}
		
		storyboard_speed += storyboard_acceleration*dt;
	}
}

void StateIntroduction::render() {
	storyboard->render(storyboard_position.x(0), storyboard_position.x(1));
	SDLBase::renderFade();
	
	truck->render(truck_position.x(0), 6480 + storyboard_position.x(1) + truck_position.x(1));
	eatles->render(eatles_position.x(0), 6480 + storyboard_position.x(1) + eatles_position.x(1));
	
	if ((stopwatch_end.time() > PRESS_DELAY) && (!pressed_enter)) {
		press->render(640, 390, true);
		return;
	}
	
	// main menu fade in
	if (stopwatch_fadein.time() > FADEIN_START) {
		fadein_alpha += MENU_FADEIN_ALPHA_DIFF;
		if (fadein_alpha <= 0)
			fadein_alpha = 0;
		bg->setAlpha(fadein_alpha);
		mainmenu->render();
		bg->render();
	}
}

void StateIntroduction::updateEatles() {
	Scalar dt = float(SDLBase::dt())/1000;
	
	if (stopwatch_eatles.time() > EATLES_GETUP + eatles_states[1]->getTimeSize() + EATLES_WALK)
		eatles_position += eatles_speed*dt;
	else if (stopwatch_eatles.time() > EATLES_GETUP + eatles_states[1]->getTimeSize()) {
		if (!played_sound) {
			sound_eatles_getup->play(1);
			played_sound = true;
		}
		eatles = eatles_states[2];
		stopwatch_fadein.start();
		fadein_alpha = 1;
		return;
	}
	else if (stopwatch_eatles.time() > EATLES_GETUP) {
		eatles = eatles_states[1];
		eatles_position = r2vec(230, 310);
	}
	else if (stopwatch_eatles.time() >= 0)
		return;
	
	eatles->update();
}

void StateIntroduction::handleKeyDown(const observer::Event& event, bool& stop) {
	switch (inputmanager_event.key.keysym.sym) {
	case SDLK_RETURN:
	case SDLK_KP_ENTER:
		if (stopwatch_end.time() <= PRESS_DELAY)
			throw new Change("StateMainMenu");
		else if (!pressed_enter) {
			played_sound = false;
			pressed_enter = true;
			stopwatch_eatles.start();
		}
		break;
		
	default:
		throw new Change("StateMainMenu");
		break;
	}
}
