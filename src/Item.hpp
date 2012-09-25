
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
		TIME = 0,
		POINT,
		LIFE,
		MASS,
		KEY
	};
	
	class Event : public observer::Event {
	private:
		char op;
		int val;
	public:
		Event(char operation, int value);
		char operation() const;
		int value() const;
	};
	
	char operation;
	int value;
	
	Item();
	
	void checkAvatarCollision(GameObject* avatar, bool& enable);
};

#endif