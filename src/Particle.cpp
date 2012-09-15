#include "Particle.hpp"

#include "Circle.hpp"
#include "SDLBase.hpp"
#include "FieldForce.hpp"

using namespace lalge;

Particle::Particle() : elasticity(0), mass(1), charge(0), pinned(false) { setShape(new Circle()); }
Particle::~Particle() {}

void Particle::update() {
	if (pinned)
		return;
	
	Scalar dt = Scalar(SDLBase::dt())/1000;
	getShape()->position += ((speed*dt) + acceleration*(dt*dt/2));
	speed += (acceleration*dt);
	acceleration = force/mass;
	force.annul();
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

void Particle::addParticleFieldForces(GameObject* target) {
	force += gravitationalForce(*((Particle*)target));
	force += electricalForce(*((Particle*)target));
	// FIXME: someday
	//force += magneticForce(*target);
}

R2Vector Particle::gravitationalForce(const Particle& target) const {
	R2Vector range = getShape()->range(*target.getShape());
	Scalar size = (grav_const*mass*target.mass)/pow(range.size(), 3);
	return (range*size);
}

R2Vector Particle::electricalForce(const Particle& target) const {
	R2Vector range = getShape()->range(*target.getShape());
	Scalar size = (elec_const*charge*target.charge)/pow(range.size(), 3);
	return (range*size);
}

R2Vector Particle::magneticForce(const Particle& target) const {
	return R2Vector();	// FIXME: someday
}
