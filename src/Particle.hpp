
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "GameObject.hpp"
#include "Audio.hpp"

class Particle : public GameObject {
private:
	lalge::Scalar elasticity;
	lalge::Scalar mass;
protected: 
	lalge::Scalar charge;
public:
	lalge::R2Vector speed;
	lalge::R2Vector acceleration;
	lalge::R2Vector force;
	
	static Audio* sound_collision_elastic;
	static Audio* sound_collision_inelastic;
	
	bool pinned;
	
	Particle();
	virtual ~Particle();
	
	virtual void update();
	
	GameObject* clone();
	
	lalge::Scalar getElasticity() const;
	void setElasticity(lalge::Scalar elasticity);
	
	lalge::Scalar getMass() const;
	void setMass(lalge::Scalar mass);
	void addMass(lalge::Scalar plus);
	
	virtual lalge::Scalar getCharge() const;
	virtual void setCharge(lalge::Scalar charge);
	
	void manageParticleCollision(GameObject* target, bool& enable);
	bool collides(const Particle& target) const;
	
	void addParticleFieldForces(GameObject* target, bool& enable);
	lalge::R2Vector gravitationalForce(const Particle& target) const;
	lalge::R2Vector electricalForce(const Particle& target) const;
	lalge::R2Vector magneticForce(const Particle& target) const;
};

#endif
