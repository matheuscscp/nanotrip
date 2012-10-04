
#ifndef BORDER_HPP
#define BORDER_HPP

#include "Particle.hpp"

class Border : public GameObject {
private:
	lalge::Scalar elasticity;
public:
	Border();
	
	void update();
	
	lalge::Scalar getElasticity() const;
	void setElasticity(lalge::Scalar elasticity);
	
	void particleCollision(GameObject* particle, bool& enable);
	bool collides(const Particle& particle) const;
};

#endif
