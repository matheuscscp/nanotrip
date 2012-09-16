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
	force = 0;
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
	R2Vector range = target.getShape()->range(*getShape());
	Scalar size = (elec_const*charge*target.charge)/pow(range.size(), 3);
	return (range*size);
}

R2Vector Particle::magneticForce(const Particle& target) const {
	return R2Vector();	// FIXME: someday
}

void Particle::manageParticleCollision(GameObject* target) {
	// check collision
	if (!collides(*((Particle*)target)))
		return;
	
	// respond to collision
	R2Vector tmp = getShape()->range(*target->getShape()).unitvec();
	
	Scalar k_res = elasticity + ((Particle*)target)->elasticity;
	
	Scalar m1 = mass;
	Scalar m2 = ((Particle*)target)->mass;
	Scalar M = m1 + m2;
	
	Scalar v1i = tmp * speed;
	Scalar v2i = tmp * ((Particle*)target)->speed;
	
	Scalar v1f = ((v1i*(m1-m2))	+ (v2i*2*m2)	)/M;
	Scalar v2f = ((v1i*2*m1)	+ (v2i*(m2-m1))	)/M;
	
	R2Vector normal = rotate(90, tmp);
	
	speed						= ((tmp * v1f) + normal.proj(speed)							) * k_res;
	((Particle*)target)->speed	= ((tmp * v2f) + normal.proj(((Particle*)target)->speed)	) * k_res;
}

bool Particle::collides(const Particle& target) const {
	// obvious collision test
	if (((Circle*)getShape())->collides(*((Circle*)target.getShape())))
		return true;
	
	// collision test for the next position
	Scalar dt = Scalar(SDLBase::dt())/1000;
	Circle c1(*((Circle*)getShape()));
	Circle c2(*((Circle*)target.getShape()));
	R2Vector c1_dpos;
	R2Vector c2_dpos;
	if (!pinned)	// pinned particles don't move
		c1_dpos = ((speed*dt) + acceleration*(dt*dt/2));
	if (!target.pinned)	// pinned particles don't move
		c2_dpos = ((target.speed*dt) + target.acceleration*(dt*dt/2));
	if ((!c1_dpos.size()) && (!c2_dpos.size()))	// pinned particles don't collide
		return false;
	c1.position += c1_dpos;
	c2.position += c2_dpos;
	if (c1.collides(c2))
		return true;
	
	// collision test for crossing trajectories
	// RESOLVER O SISTEMA LINEAR DOS VETORES DIFF
	
	return false;
}
