#include "DraggableObject.hpp"

#include "InputManager.hpp"

using namespace lalge;

DraggableObject::DraggableObject(Shape* shape) : hooked(false) {
	setShape(shape);
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &DraggableObject::handleMouseDownLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &DraggableObject::handleMouseUpLeft);
}

DraggableObject::~DraggableObject() {
	InputManager::instance()->disconnect(this);
}

void DraggableObject::update() {
	if (hooked)
		getShape()->position = diff_position + r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
}

bool DraggableObject::isHooked() const { return hooked; }

void DraggableObject::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (getShape()->mouseDownInside()) {
		hooked = true;
		diff_position = getShape()->position - r2vec(InputManager::instance()->mouseDownX(), InputManager::instance()->mouseDownY());
	}
}

void DraggableObject::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	hooked = false;
}
