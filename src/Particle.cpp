#include "Particle.hpp"

#include "Circle.hpp"
#include "SDLBase.hpp"

using namespace lalge;

Particle::Particle() : elasticity(0), mass(1), pinned(false) { setShape(new Circle()); }
Particle::~Particle() {}

void Particle::update() {
	updateKinematic();
}

Scalar Particle::getElasticity() const { return elasticity; }

void Particle::setElasticity(Scalar elasticity) {
	if ((elasticity >= 0) && (elasticity <= 0.5))
		this->elasticity = elasticity;
}

Scalar Particle::getMass() const { return mass; }

void Particle::setMass(Scalar mass) {
	if (mass > 0)
		this->mass = mass;
}

void Particle::updateKinematic() {
	if (pinned)
		return;
	
	Scalar dt = Scalar(SDLBase::dt())/1000;
	getShape()->position += ((speed*dt) + acceleration*(dt*dt/2));
	speed += (acceleration*dt);
	acceleration = force/mass;
}
