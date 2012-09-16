#include "StateFirstLevel.hpp"

#include "Animation.hpp"
#include "Circle.hpp"

GAMESTATE_DEF(StateFirstLevel)

StateFirstLevel::StateFirstLevel(ArgsBase* args) {
	bg = new Sprite("img/bg_opaco.png");
	bg->render();
	delete bg;
	bg = new Sprite("img/bg_transparente.png");
	
	level.load("level/level.conf");
	
	level.avatar->sprite = new Animation("img/avatar.png", 0, 7, 1, 16);
	
	positive_sprite = new Sprite("img/prot.png");
	negative_sprite = new Sprite("img/elec.png");
	
	((Circle*)level.avatar->getShape())->setRadius(28);
	level.avatar->setMass(5);
	
	for (std::list<Particle*>::iterator it = level.particles.begin(); it != level.particles.end(); ++it) {
		((Circle*)(*it)->getShape())->setRadius(10);
		(*it)->pinned = true;
		if ((*it)->charge < 0)
			(*it)->sprite = negative_sprite;
		else
			(*it)->sprite = positive_sprite;
	}
}

void StateFirstLevel::update() {
}

void StateFirstLevel::render() {
}
