
#ifndef AVATAR_HPP
#define AVATAR_HPP

#include "observer.hpp"

#include "Particle.hpp"

class Avatar : public Particle {
SUBJECT
public:
	enum {
		BEINGSWALLOWED = 0,
		
		// don't change this
		LASTEVENT
	};
private:
	bool being_swallowed;
	GameObject* blackhole;
public:
	Avatar();
	
	void update();
	
	void checkBlackHoleCollision(GameObject* blackhole, bool& enable);
};

#endif
