#include "Avatar.hpp"

#include <iostream>
#include "SDLBase.hpp"

using namespace lalge;

Avatar::Avatar() : being_swallowed(false) {
	subject.init(LASTEVENT);
}

void Avatar::update() {
	if (!being_swallowed) {
		Particle::update();
		return;
	}
	
	Scalar dt = Scalar(SDLBase::dt())/1000;
	getShape()->position += ((speed*dt) + acceleration*(dt*dt/2));
	speed = getShape()->range(*(blackhole->getShape()));
	speed = speed + rotate(90, speed)*4;
}

void Avatar::checkBlackHoleCollision(GameObject* blackhole, bool& enable) {
	if (collides(*((Particle*)blackhole))) {
		enable = false;
		being_swallowed = true;
		this->blackhole = blackhole;
		subject.broadcast(observer::Event(BEINGSWALLOWED));
	}
}

void Avatar::setCharge(Scalar charge) {
	//TODO change avatar color
	if (this->sprite){
		this->sprite->tint((charge-0.03)*10); //TODO use correct charge values
	}
	this->charge = charge;
}
