#include "Item.hpp"

Item::Event::Event(char operation, float value) :
observer::Event(COLLISION), operation(operation), value(value) {}

Item::Item() : operation(0), value(0) { subject.init(LASTEVENT); }

void Item::checkAvatarCollision(GameObject* avatar, bool& enable) {
	if (!collides(*((Particle*)avatar)))
		return;
	
	// normal items collision
	if (operation != BARRIER) {
		enable = false;
		hidden = true;
	}
	// barrier collision
	else {
		pinned = (!value);	// unpin if the barrier was lethal
		manageParticleCollision(avatar, enable);
	}
	subject.broadcast(Event(operation, value));
}
