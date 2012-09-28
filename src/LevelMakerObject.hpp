
#ifndef LEVELMAKEROBJECT_HPP
#define LEVELMAKEROBJECT_HPP

#include "DraggableObject.hpp"

#define levelmakerobject_event	(*((LevelMakerObject::Event*)&event))

class LevelMakerObject : public DraggableObject {
SUBJECT
public:
	enum {
		// events
		SELECTED = 0,
		
		// don't change this
		LASTEVENT
	};
	
	enum {
		AVATAR,
		BLACKHOLE,
		KEY,
		PARTICLE,
		ITEM
	};
	
	class Event : public observer::Event {
	public:
		int type;
		GameObject* object;
		Event(int type, GameObject* object);
	};
private:
	int type;
	GameObject* object;
	bool selected;
public:
	Sprite* selection;
	
	LevelMakerObject(int type, GameObject* object);
	~LevelMakerObject();
	
	void update();
	void render();
	
	bool isSelected() const;
	
	int getType() const;
	const GameObject* getObject() const;
private:
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
};

#endif
