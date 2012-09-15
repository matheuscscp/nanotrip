
#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "GameObject.hpp"

class Particle : public GameObject {
private:
	lalge::Scalar elasticity;
	lalge::Scalar mass;
public:
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
protected:
	void updateKinematic();
};

#endif
