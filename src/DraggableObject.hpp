
#ifndef DRAGGABLEOBJECT_HPP
#define DRAGGABLEOBJECT_HPP

#include "observer.hpp"

#include "GameObject.hpp"

class DraggableObject : public GameObject {
private:
	lalge::R2Vector diff_position;
	bool hooked;
public:
	DraggableObject(Shape* shape);
	virtual ~DraggableObject();
	
	virtual void update();
	
	bool isHooked() const;
protected:
	virtual void handleMouseDownLeft(const observer::Event& event, bool& stop);
private:
	void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
