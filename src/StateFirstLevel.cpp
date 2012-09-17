#include "StateFirstLevel.hpp"

#include "Animation.hpp"
#include "Circle.hpp"

GAMESTATE_DEF(StateFirstLevel)

StateFirstLevel::StateFirstLevel(ArgsBase* args) {
	// only one opaque background renderization
	bg = new Sprite("img/bg_opaco.png");
	bg->render();
	delete bg;
	
	bg = new Sprite("img/bg_transparente.png");
	
	level.load("level/level.conf");
	
	level.avatar->sprite = new Animation("img/avatar.png", 0, 7, 1, 16);
	
	positive_sprite = new Sprite("img/prot.png");
	negative_sprite = new Sprite("img/elec.png");
	
	// avatar parameters
	((Circle*)level.avatar->getShape())->setRadius(level.avatar->sprite->rectW()/2);
	level.avatar->setMass(1);
	
	// for all particles
	for (std::list<Particle*>::iterator it = level.particles.begin(); it != level.particles.end(); ++it) {
		(*it)->pinned = true;
		// set radius and sprite for protons
		if ((*it)->charge > 0) {
			(*it)->sprite = positive_sprite;
			((Circle*)(*it)->getShape())->setRadius(positive_sprite->srcW()/2);
		}
		// set radius and sprite for electrons
		else {
			(*it)->sprite = negative_sprite;
			((Circle*)(*it)->getShape())->setRadius(negative_sprite->srcW()/2);
		}
	}
}

void StateFirstLevel::update() {
}

void StateFirstLevel::render() {
}
