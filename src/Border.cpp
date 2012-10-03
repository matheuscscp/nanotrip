#include "Border.hpp"

Border::Border() { setShape(new Line()); }

void Border::update() {}

lalge::Scalar Border::getElasticity() const { return elasticity; }

void Border::setElasticity(lalge::Scalar elasticity) {
	if ((elasticity >= 0) && (elasticity <= 0.5))
		this->elasticity = elasticity;
}

void Border::manageParticleCollision(GameObject* particle, bool& enable) {
	if (!collides(*((Particle*)particle)))
		return;
	
	
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
