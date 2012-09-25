#include "Item.hpp"

Item::Event::Event(char operation, int value) :
observer::Event(COLLISION), op(operation), val(value) {}
char Item::Event::operation() const { return op; }
int Item::Event::value() const { return val; }

Item::Item() : operation(0), value(0) { subject.init(LASTEVENT); }

void Item::checkAvatarCollision(GameObject* avatar, bool& enable) {
	if (collides(*((Particle*)avatar))) {
		if (value == BARRIER)
			manageParticleCollision(avatar, enable);
		else {
			enable = false;
			hidden = true;
		}
		subject.broadcast(Event(operation, value));
	}
}
