#include "StateTest.hpp"

#include "Animation.hpp"

GAMESTATE_DEF(StateTest)

Sprite* avatar;

StateTest::StateTest(ArgsBase* args) {
	bg = new Sprite("img/bg.png");
	avatar = new Animation("img/avatar.png", 0, 143, 1, 17);
}

void StateTest::update() {
}

void StateTest::render() {
	avatar->render(400, 400);
}
