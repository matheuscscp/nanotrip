#include "Item.hpp"

#include "Circle.hpp"
#include "Avatar.hpp"

Audio* Item::sound_key = 0;
Audio* Item::sound_time = 0;
Audio* Item::sound_point = 0;
Audio* Item::sound_life = 0;
Audio* Item::sound_mass = 0;
Audio* Item::sound_barrier = 0;
Audio* Item::sound_lethal_barrier = 0;

Item::Event::Event(char operation, float value) :
observer::Event(COLLISION), operation(operation), value(value) {}

Item::Item() : operation(0), value(0) { subject.init(LASTEVENT); }

GameObject* Item::clone() {
	Circle* my_shape = new Circle(*((Circle*)getShape()));
	Circle* new_shape = new Circle(*((Circle*)getShape()));
	setShape(0);
	GameObject* new_item = new Item(*this);
	setShape(my_shape);
	new_item->setShape(new_shape);
	
	return new_item;
}

float Item::getValue() const { return value; }

void Item::setValue(float value) {
	this->value = ((value < 0) ? -value : value);
}

void Item::particleCollision(GameObject* target, bool& enable) {
	if (!collides(*((Particle*)target)))
		return;
	
	Avatar* avatar = dynamic_cast<Avatar*>(target);
	
	// non-barrier item special cases
	if (operation != BARRIER) {
		// target ain't the avatar
		if (!avatar) {
			Particle::particleCollision(target, enable);
			return;
		}
		// target is the avatar, but the game is over (avatar wins)
		else if (avatar->win_lose == Avatar::WIN)
			return;
		// target is the avatar, but the game is over (avatar wins)
		else if (avatar->win_lose == Avatar::LOSE) {
			Particle::particleCollision(target, enable);
			return;
		}
	}
	// barrier item cases
	else {
		bool play_normal_collision_sound = false;
		
		// non-avatar collision
		if (!avatar)
			Particle::particleCollision(target, play_normal_collision_sound);
		// avatar wins (no collision)
		else if (avatar->win_lose == Avatar::WIN)
			return;
		// normal avatar collision
		else if (!avatar->win_lose) {
			// broadcast lose event for lethal barriers
			if (value) {
				pinned = false;
				subject.broadcast(Event(operation, value));
			}
			Particle::particleCollision(target, play_normal_collision_sound);
		}
		// avatar already lost collision
		else
			Particle::particleCollision(target, play_normal_collision_sound);
		
		// play barrier sound
		if ((!value) && (sound_barrier))
			sound_barrier->play(1);
		else if ((value) && (sound_lethal_barrier))
			sound_lethal_barrier->play(1);
		
		return;
	}
	
	// non-barrier item-avatar normal collision (something special must happen)
	
	enable = false;
	hidden = true;
	
	// play sound, or return (if the operation was invalid)
	switch (operation) {
	case KEY:
		if (sound_key)
			sound_key->play(1);
		break;
		
	case TIME:
		if (sound_time)
			sound_time->play(1);
		break;
		
	case POINT:
		if (sound_point)
			sound_point->play(1);
		break;
		
	case LIFE:
		if (sound_life)
			sound_life->play(1);
		break;
		
	case MASS:
		if (sound_mass)
			sound_mass->play(1);
		break;
		
	default:
		return;
	}
	
	subject.broadcast(Event(operation, value));
}
