#include "StateTest.hpp"

#include "common.hpp"

#include "Animation.hpp"
#include "Particle.hpp"
#include "Circle.hpp"

using namespace lalge;
using namespace common;

GAMESTATE_DEF(StateTest)

Particle* avatar;

Sprite* bg1;

StateTest::StateTest(ArgsBase* args) {
	bg1 = new Sprite("img/bg_opaco.png");
	bg1->render();
	bg = new Sprite("img/bg_transparente.png");
	avatar = new Particle();
	avatar->sprite = new Animation("img/avatar.png", 0, 7, 1, 16);
	((Circle*)avatar->getShape())->setRadius(100);
	avatar->getShape()->position = r2vec(100, 100);
	avatar->speed = r2vec(300, 150);
}

void StateTest::update() {
}

void StateTest::render() {
	GameObject::renderAll();
}
