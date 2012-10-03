
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
	
	static Audio* sound_key;
	static Audio* sound_time;
	static Audio* sound_point;
	static Audio* sound_life;
	static Audio* sound_mass;
	static Audio* sound_barrier;
	static Audio* sound_lethal_barrier;
private:
	float value;
public:
	char operation;
	
	Item();
	
	GameObject* clone();
	
	float getValue() const;
	void setValue(float value);
	
	void particleCollision(GameObject* target, bool& enable);
};

#endif
