#include "LevelMakerObject.hpp"

#include "Circle.hpp"
#include "InputManager.hpp"

using namespace lalge;

using std::set;

set<LevelMakerObject*> LevelMakerObject::selected;
Sprite* LevelMakerObject::LevelMakerObject::sprite_selection_box = 0;
Sprite* LevelMakerObject::sprite_selection_horizontal = 0;
Sprite* LevelMakerObject::sprite_selection_vertical = 0;
set<LevelMakerObject*> LevelMakerObject::all;
bool LevelMakerObject::dragging = false;
bool LevelMakerObject::selecting = false;
Rectangle LevelMakerObject::selection_box;

LevelMakerObject::LevelMakerObject(int type, GameObject* object) :
type(type),
object(object),
selection(0)
{
	all.insert(this);
	setShape(new Circle(*((Circle*)object->getShape())));
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &LevelMakerObject::handleMouseDownLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &LevelMakerObject::handleMouseUpLeft);
}

LevelMakerObject::~LevelMakerObject() {
	selected.erase(this);
	all.erase(this);
	delete object;
	InputManager::instance()->disconnect(this);
}

void LevelMakerObject::updateSelected() {
	if (!dragging)
		return;
	
	for (set<LevelMakerObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
		(*it)->update();
}

void LevelMakerObject::updateSelection() {
	if (!selecting)
		return;
	
	// the box
	{
		int x = InputManager::instance()->mouseDownX() + InputManager::instance()->mouseDiffX()/2;
		int y = InputManager::instance()->mouseDownY() + InputManager::instance()->mouseDiffY()/2;
		selection_box.position = r2vec(x, y);
		selection_box.setWidth(ABSVAL(InputManager::instance()->mouseDiffX()));
		selection_box.setHeight(ABSVAL(InputManager::instance()->mouseDiffY()));
		
		if (sprite_selection_box)
			sprite_selection_box->clip(0, 0, selection_box.getWidth(), selection_box.getHeight());
		if (sprite_selection_horizontal)
			sprite_selection_horizontal->clip(0, 0, selection_box.getWidth(), 1);
		if (sprite_selection_vertical)
			sprite_selection_vertical->clip(0, 0, 1, selection_box.getHeight());
	}
	
	// select objects
	for (set<LevelMakerObject*>::iterator it = all.begin(); it != all.end(); ++it) {
		if (selection_box.Shape::pointInside((*it)->getShape()->position))
			selected.insert(*it);
	}
	
	// deselect objects
	for (set<LevelMakerObject*>::iterator it = selected.begin(); it != selected.end(); ++it) {
		if (!selection_box.Shape::pointInside((*it)->getShape()->position))
			selected.erase(*it);
	}
}

void LevelMakerObject::renderSelection() {
	if (!selecting)
		return;
	
	if (sprite_selection_box)
		sprite_selection_box->render(selection_box.position.x(0), selection_box.position.x(1), true);
	
	if (sprite_selection_horizontal) {
		// upper line
		int x = selection_box.position.x(0) - selection_box.getWidth()/2;
		int y = selection_box.position.x(1) - selection_box.getHeight()/2;
		sprite_selection_horizontal->render(x, y);
		
		// lower line
		y += selection_box.getHeight();
		sprite_selection_horizontal->render(x, y);
	}
	
	if (sprite_selection_vertical) {
		// right line
		int x = selection_box.position.x(0) - selection_box.getWidth()/2;
		int y = selection_box.position.x(1) - selection_box.getHeight()/2;
		sprite_selection_vertical->render(x, y);
		
		// left line
		x += selection_box.getWidth();
		sprite_selection_vertical->render(x, y);
	}
}

int LevelMakerObject::getSelectedType() {
	if (!selected.size())
		return NONE;
	
	int type = (*selected.begin())->getType();
	for (set<LevelMakerObject*>::iterator it = selected.begin(); it != selected.end(); ++it) {
		if ((*it)->getType() != type)
			return NONE;
	}
	return type;
}

void LevelMakerObject::update() {
	getShape()->position = mouse_down_position + r2vec(InputManager::instance()->mouseDiffX(), InputManager::instance()->mouseDiffY());
	object->getShape()->position = getShape()->position;
}

void LevelMakerObject::render() {
	if (hidden)
		return;
	
	object->render();
	if ((isSelected()) && (selection))
		selection->render(getShape()->position.x(0), getShape()->position.x(1), true);
}

bool LevelMakerObject::isSelected() const {
	return (selected.find((LevelMakerObject*)this) != selected.end());
}

int LevelMakerObject::getType() const { return type; }
const GameObject* LevelMakerObject::getObject() const { return object; }

bool LevelMakerObject::mouseDownInsideAny() {
	for (set<LevelMakerObject*>::iterator it = all.begin(); it != all.end(); ++it) {
		if ((*it)->getShape()->mouseDownInside())
			return true;
	}
	return false;
}

void LevelMakerObject::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (selecting)
		return;
	
	mouse_down_position = getShape()->position;
	
	if (!getShape()->mouseDownInside()) {
		// if clicked outside every object, deselect this object and begin multiple selection
		if (!mouseDownInsideAny()) {
			selected.clear();
			selecting = true;
			selection_box.setWidth(1);
			selection_box.setHeight(1);
		}
		return;
	}
	
	dragging = true;
	
	// if this isn't selected, clear the selection and select only this object
	if (!isSelected()) {
		selected.clear();
		selected.insert(this);
	}
}

void LevelMakerObject::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	dragging = false;
	selecting = false;
}
