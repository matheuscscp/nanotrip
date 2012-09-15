#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "observer.hpp"

#include "GameObject.hpp"

class Button : public GameObject {
SUBJECT
public:
	enum {
		CLICKED = 0,
		
		// don't change this
		LASTEVENT
	};
private:
	bool clicked;
	bool active;
public:
	Sprite* sprite;
	
	Button(const lalge::R2Vector& r, Sprite* sprite, bool flag = true);
	~Button();
	
	void render();
	
	void enable(bool flag);
private:
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
	void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
