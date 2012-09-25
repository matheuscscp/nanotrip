
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "GameObject.hpp"

class Particle : public GameObject {
private:
	lalge::Scalar elasticity;
	lalge::Scalar mass;
public:
	lalge::Scalar charge;
	
	lalge::R2Vector speed;
	lalge::R2Vector acceleration;
	lalge::R2Vector force;
	
	bool pinned;
	
	Particle();
	virtual ~Particle();
	
	virtual void update();
	
	lalge::Scalar getElasticity() const;
	void setElasticity(lalge::Scalar elasticity);
	
	lalge::Scalar getMass() const;
	void setMass(lalge::Scalar mass);
	void addMass(lalge::Scalar plus);
	
	void manageParticleCollision(GameObject* target, bool& enable);
	bool collides(const Particle& target) const;
	
	void addParticleFieldForces(GameObject* target, bool& enable);
	lalge::R2Vector gravitationalForce(const Particle& target) const;
	lalge::R2Vector electricalForce(const Particle& target) const;
	lalge::R2Vector magneticForce(const Particle& target) const;
};

#endif
