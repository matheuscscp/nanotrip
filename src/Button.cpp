#include "Button.hpp"

#include "Rectangle.hpp"
#include "InputManager.hpp"

using namespace lalge;

Audio* Button::sound_hover = 0;
Audio* Button::sound_clicked = 0;

Button::Button(Sprite* sprite) :
clip_y(0),
clicked(false),
hover(false),
just_clicked(false),
just_hit(false)
{
	subject.init(LASTEVENT);
	setShape(new Rectangle());
	enable(true);
	
	// width and height by sprite
	this->sprite = sprite;
	((Rectangle*)getShape())->setWidth(sprite->srcW());
	((Rectangle*)getShape())->setHeight(sprite->srcH()/4);
}

Button::~Button() {
	InputManager::instance()->disconnect(this);
}

void Button::update() {
	if (!enabled)
		return;
	
	if (clicked) {
		clicked = false;
		clip_y = 2*sprite->srcH()/4;
		hover = false;
	}
	else if (!getShape()->mouseInside()) {
		clip_y = 0;
		hover = false;
		just_hit = false;
	}
	else if (!InputManager::instance()->mousePressed(SDL_BUTTON_LEFT)) {
		clip_y = sprite->srcH()/4;
		
		// play sound
		if ((sound_hover) && (!hover)) {
			if (!just_clicked)
				sound_hover->play(1);
			else
				just_clicked = false;
		}
		
		hover = true;
		just_hit = false;
	}
	else if (getShape()->mouseDownInside()) {
		clip_y = 2*sprite->srcH()/4;
		hover = false;
	}
	else {
		clip_y = 0;
		hover = false;
		just_hit = false;
	}
	
	sprite->clip(0, clip_y, sprite->srcW(), sprite->srcH()/4);
}

void Button::enable(bool enable) {
	enabled = enable;
	if (!enable) {
		sprite->clip(0, 3*sprite->srcH()/4, sprite->srcW(), sprite->srcH()/4);
		InputManager::instance()->disconnect(this);
	}
	else {
		InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &Button::handleMouseDownLeft);
		InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &Button::handleMouseUpLeft);
	}
}

void Button::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if ((getShape()->mouseDownInside()) && (enabled)) {
		clicked = true;
		
		// play sound
		if (sound_clicked) {
			sound_clicked->play(1);
			just_hit = true;
		}
	}
}

void Button::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	if ((getShape()->mouseDownInside()) && (getShape()->mouseInside()) && (enabled)) {
		clicked = true;
		subject.broadcast(observer::Event(CLICKED));
		just_clicked = true;
		
		// play sound
		if ((sound_clicked) && (!just_hit))
			sound_clicked->play(1);
	}
}
