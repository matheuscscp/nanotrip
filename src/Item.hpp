
#ifndef ITEM_HPP
#define ITEM_HPP

#include "observer.hpp"

#include "Particle.hpp"

class Item : public Particle {
SUBJECT
public:
	enum {
		COLLISION = 0,
		
		// don't change this
		LASTEVENT
	};
	
	enum type {
		KEY = 0,
		TIME,	// 1
		POINT,	// 2
		LIFE,	// 3
		MASS,	// 4
		BARRIER	// 5
	};
	
	class Event : public observer::Event {
	public:
		char operation;
		float value;
		Event(char operation, float value);
	};
	
	char operation;
	float value;
	
	Item();
	
	void checkAvatarCollision(GameObject* avatar, bool& enable);
};

#endif
