#include "Border.hpp"

#include "Line.hpp"
#include "SDLBase.hpp"

using namespace lalge;

Border::Border() { setShape(new Line()); }

void Border::update() {}

Scalar Border::getElasticity() const { return elasticity; }

void Border::setElasticity(Scalar elasticity) {
	if ((elasticity >= 0) && (elasticity <= 0.5))
		this->elasticity = elasticity;
}

void Border::particleCollision(GameObject* particle, bool& enable) {
	if (!collides(*((Particle*)particle)))
		return;
	
	try {
		R2Vector speed_projection = ((Line*)getShape())->getDirection().proj(((Particle*)particle)->speed);
		((Particle*)particle)->speed += (speed_projection - ((Particle*)particle)->speed)*2;
	}
	catch (ProjectionNotDefined&) {
		((Particle*)particle)->speed *= -1;
	}
	
	// play sound
	if (elasticity + ((Particle*)particle)->getElasticity() > 0.5) {
		if (Particle::sound_collision_elastic)
			Particle::sound_collision_elastic->play(1);
		else if (Particle::sound_collision_inelastic)
			Particle::sound_collision_inelastic->play(1);
	}
	else {
		if (Particle::sound_collision_inelastic)
			Particle::sound_collision_inelastic->play(1);
		else if (Particle::sound_collision_elastic)
			Particle::sound_collision_elastic->play(1);
	}
}

bool Border::collides(const Particle& particle) const {
	// pinned particle won't collide
	if (particle.pinned)
		return false;
	
	// obvious test
	if (((Line*)getShape())->collides(*((Circle*)particle.getShape())))
		return true;
	
	// next position test
	Scalar dt = Scalar(SDLBase::dt())/1000;
	Circle circle(*((Circle*)particle.getShape()));
	R2Vector circle_dpos;
	circle_dpos = ((particle.speed*dt) + particle.acceleration*(dt*dt/2));
	circle.position += circle_dpos;
	if (((Line*)getShape())->collides(circle))
		return true;
	
	// crossing trajectory
	// RESOLVER O SISTEMA LINEAR DOS VETORES DIFERENCA e tal
	
	
	return false;
}
