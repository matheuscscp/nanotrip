#include "Avatar.hpp"

#include "SDLBase.hpp"
#include "Circle.hpp"

#include "common.hpp"

using namespace lalge;

Avatar::Avatar() : being_swallowed(false), win_lose(NONE) {
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
	
	// rotozoom
	{
		Scalar zoom = speed.size()/((Circle*)blackhole->getShape())->getRadius();
		zoom = ((zoom > 1) ? 1 : ((zoom < 0.0000001) ? 0.0000001 : zoom));
		sprite->rotozoom(0, zoom, zoom);
		((Circle*)getShape())->setRadius(sprite->rectW()/2);
	}
	
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
	this->charge = charge;
	if (this->sprite){
		this->sprite->tint((this->charge-0.03)*10); //TODO use correct charge values
	}
}
