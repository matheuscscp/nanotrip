#include "Particle.hpp"

#include "Circle.hpp"
#include "SDLBase.hpp"
#include "FieldForce.hpp"

using namespace lalge;

Audio* Particle::sound_collision_elastic = 0;
Audio* Particle::sound_collision_inelastic = 0;

Particle::Particle() : elasticity(0), mass(1), charge(0), pinned(false) { setShape(new Circle()); }
Particle::~Particle() {}

void Particle::update() {
	if (pinned) {
		force = 0;
		return;
	}
	
	Scalar dt = Scalar(SDLBase::dt())/1000;
	getShape()->position += ((speed*dt) + acceleration*(dt*dt/2));
	speed += (acceleration*dt);
	acceleration = force/mass;
	force = 0;
}

GameObject* Particle::clone() {
	Circle* my_shape = new Circle(*((Circle*)getShape()));
	Circle* new_shape = new Circle(*((Circle*)getShape()));
	setShape(0);
	GameObject* new_particle = new Particle(*this);
	setShape(my_shape);
	new_particle->setShape(new_shape);
	
	return new_particle;
}

Scalar Particle::getElasticity() const { return elasticity; }

void Particle::setElasticity(Scalar elasticity) {
	if ((elasticity >= 0) && (elasticity <= 0.5))
		this->elasticity = elasticity;
}

Scalar Particle::getCharge() const { return charge; }

void Particle::setCharge(Scalar charge) {
	this->charge = charge;
}

Scalar Particle::getMass() const { return mass; }

void Particle::setMass(Scalar mass) {
	if (mass > 0)
		this->mass = mass;
}

void Particle::addMass(lalge::Scalar plus) {
	if (mass + plus > 0)
		mass += plus;
}

void Particle::manageParticleCollision(GameObject* target, bool& enable) {
	// check collision
	if ((hidden) || (!collides(*((Particle*)target))))
		return;
	
	// respond to collision
	R2Vector tmp = getShape()->range(*target->getShape());
	if (tmp.size())
		tmp = tmp.unitvec();
	else {
		getShape()->position.add(0, 50);
		tmp = getShape()->range(*target->getShape()).unitvec();
	}
	
	Scalar k_res = elasticity + ((Particle*)target)->elasticity;
	
	Scalar m1 = mass;
	Scalar m2 = ((Particle*)target)->mass;
	Scalar M = m1 + m2;
	
	// pinned particles have null speed
	Scalar v1i = ((!pinned) ? tmp * speed : 0);
	// pinned particles have null speed
	Scalar v2i = ((!((Particle*)target)->pinned) ? tmp * ((Particle*)target)->speed : 0);
	
	Scalar v1f = ((v1i*(m1-m2))	+ (v2i*2*m2)	)/M;
	Scalar v2f = ((v1i*2*m1)	+ (v2i*(m2-m1))	)/M;
	
	R2Vector normal = rotate(90, tmp);
	
	// only not pinned particles should update the speed
	if (!pinned)
		speed = ((tmp * v1f) + normal.proj(speed)) * k_res;
	// only not pinned particles should update the speed
	if (!((Particle*)target)->pinned)
		((Particle*)target)->speed = ((tmp * v2f) + normal.proj(((Particle*)target)->speed)) * k_res;
	
	// play sound
	if (k_res > 0.5) {
		if (sound_collision_elastic)
			sound_collision_elastic->play(1);
		else if (sound_collision_inelastic)
			sound_collision_inelastic->play(1);
	}
	else {
		if (sound_collision_inelastic)
			sound_collision_inelastic->play(1);
		else if (sound_collision_elastic)
			sound_collision_elastic->play(1);
	}
}

bool Particle::collides(const Particle& target) const {
	// pinned particles don't collide
	if ((pinned) && (target.pinned))
		return false;
	
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
	c1.position += c1_dpos;
	c2.position += c2_dpos;
	if (c1.collides(c2))
		return true;
	
	// collision test for crossing trajectories
	// RESOLVER O SISTEMA LINEAR DOS VETORES DIFERENCA e tal
	
	
	return false;
}

void Particle::addParticleFieldForces(GameObject* target, bool& enable) {
	if (hidden)
		return;
	
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
	// There is no obvious expression to calculate magnetic force between
	// particles. So we're going to despise it in this game.
	return R2Vector();
}
