#include "StateTest.hpp"

#include "InputManager.hpp"
#include "Animation.hpp"

// for the game state ID and constructor builder
GAMESTATE_DEF(StateTest)

Sprite* bg;
Animation* avatar;

StateTest::StateTest(ArgsBase* args) {
	bg = new Sprite("img/bg.png");
	avatar = new Animation("img/avatar.png", 0, 143, 1, 17);
}

StateTest::~StateTest() {
	delete bg;
	delete avatar;
}

void StateTest::update() {
	avatar->update();
}

void StateTest::render() {
	bg->render();
	avatar->render(400, 400);
}
