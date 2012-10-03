#include "Item.hpp"

#include "Circle.hpp"

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

void Item::checkAvatarCollision(GameObject* avatar, bool& enable) {
	if (!collides(*((Particle*)avatar)))
		return;
	
	enable = false;
	hidden = true;
	
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
		
	case BARRIER:
		enable = true;
		hidden = false;
		
		if (!value) {
			if (sound_barrier)
				sound_barrier->play(1);
		}
		else {
			if (sound_lethal_barrier)
				sound_lethal_barrier->play(1);
			pinned = false;
		}
		manageParticleCollision(avatar, enable);
		break;
		
	default:
		return;
	}
	
	subject.broadcast(Event(operation, value));
}
