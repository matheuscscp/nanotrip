#include "LevelMakerObject.hpp"

#include "Circle.hpp"

LevelMakerObject::Event::Event(int type, GameObject* object) :
observer::Event(SELECTED), type(type), object(object) {}

LevelMakerObject::LevelMakerObject(int type, GameObject* object) :
DraggableObject(new Circle(*((Circle*)object->getShape()))),
type(type),
object(object),
selected(false),
selection(0)
{
	subject.init(LASTEVENT);
}

LevelMakerObject::~LevelMakerObject() {
	delete object;
}

void LevelMakerObject::update() {
	DraggableObject::update();
	object->getShape()->position = getShape()->position;
}

void LevelMakerObject::render() {
	if (hidden)
		return;
	
	if ((selected) && (selection))
		selection->render(getShape()->position.x(0), getShape()->position.x(1), true);
	
	object->render();
}

bool LevelMakerObject::isSelected() const { return selected; }
int LevelMakerObject::getType() const { return type; }
const GameObject* LevelMakerObject::getObject() const { return object; }

void LevelMakerObject::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	DraggableObject::handleMouseDownLeft(event, stop);
	if (!getShape()->mouseDownInside())
		selected = false;
	else {
		selected = true;
		subject.broadcast(Event(type, object));
	}
}
