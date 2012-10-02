#include "LevelMakerObject.hpp"

#include "Circle.hpp"
#include "InputManager.hpp"

using namespace lalge;

using std::set;

set<LevelMakerObject*> LevelMakerObject::selected;
bool LevelMakerObject::deselection_requested = false;
bool LevelMakerObject::selection_requested = false;
bool LevelMakerObject::toggle_requested = false;
LevelMakerObject* LevelMakerObject::mousedown_requested = 0;
Sprite* LevelMakerObject::LevelMakerObject::sprite_selection_box = 0;
Sprite* LevelMakerObject::sprite_selection_horizontal = 0;
Sprite* LevelMakerObject::sprite_selection_vertical = 0;
set<LevelMakerObject*> LevelMakerObject::all;
bool LevelMakerObject::dragging = false;
bool LevelMakerObject::selecting = false;
bool LevelMakerObject::just_selected = false;
bool LevelMakerObject::selected_on_click = false;
Rectangle LevelMakerObject::selection_box;
R2Vector LevelMakerObject::mouse_ctrl_position;

LevelMakerObject::LevelMakerObject(int type, GameObject* object) :
type(type),
object(object),
selection(0),
just_created(false)
{
	all.insert(this);
	setShape(new Circle(*((Circle*)object->getShape())));
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &LevelMakerObject::handleMouseDownLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &LevelMakerObject::handleMouseUpLeft);
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &LevelMakerObject::handleKeyDown);
	InputManager::instance()->connect(InputManager::KEYUP, this, &LevelMakerObject::handleKeyUp);
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

void LevelMakerObject::deselect() {
	deselection_requested = false;
	selected.clear();
}

void LevelMakerObject::startSelection() {
	selection_requested = false;
	selecting = true;
	selection_box.setWidth(1);
	selection_box.setHeight(1);
}

void LevelMakerObject::toggle() {
	toggle_requested = false;
	
	// deselect the only one selected object
	if ((selected.size() == 1) && (selected_on_click))
		selected.clear();
	// select only one object (mouse inside)
	else {
		LevelMakerObject* target;
		for (set<LevelMakerObject*>::iterator it = selected.begin(); it != selected.end(); ++it) {
			if ((*it)->getShape()->mouseInside()) {
				target = *it;
				break;
			}
		}
		selected.clear();
		selected.insert(target);
	}
}

void LevelMakerObject::handleMouseDown() {
	dragging = true;
	
	// if this isn't selected, clear the selection and select only this object
	selected_on_click = mousedown_requested->isSelected();
	if (!selected_on_click) {
		selected.clear();
		selected.insert(mousedown_requested);
	}
	
	mousedown_requested = 0;
}

void LevelMakerObject::update() {
	if ((InputManager::instance()->keyPressed(SDLK_LCTRL)) ||
		(InputManager::instance()->keyPressed(SDLK_RCTRL)))
		getShape()->position = mouse_down_position + r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY()) - mouse_ctrl_position;
	else
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

LevelMakerObject* LevelMakerObject::clone() {
	GameObject* new_gameobject = object->clone();
	LevelMakerObject* new_object = new LevelMakerObject(type, new_gameobject);
	new_object->selection = selection;
	new_object->mouse_down_position = mouse_down_position;
	return new_object;
}

bool LevelMakerObject::isSelected() const {
	return (selected.find((LevelMakerObject*)this) != selected.end());
}

int LevelMakerObject::getType() const { return type; }
GameObject* LevelMakerObject::getGameObject() const { return object; }

bool LevelMakerObject::mouseInsideAny() {
	for (set<LevelMakerObject*>::iterator it = all.begin(); it != all.end(); ++it) {
		if ((*it)->getShape()->mouseInside())
			return true;
	}
	return false;
}

bool LevelMakerObject::mouseDownInsideAny() {
	for (set<LevelMakerObject*>::iterator it = all.begin(); it != all.end(); ++it) {
		if ((*it)->getShape()->mouseDownInside())
			return true;
	}
	return false;
}

void LevelMakerObject::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (selection_requested)
		return;
	
	mouse_down_position = getShape()->position;
	
	if (!getShape()->mouseDownInside()) {
		// if clicked outside every object, request for selection
		if ((!mouseDownInsideAny()) &&
			(!InputManager::instance()->keyPressed(SDLK_LCTRL)) &&
			(!InputManager::instance()->keyPressed(SDLK_RCTRL))) {
			selection_requested = true;
			just_selected = false;
		}
		return;
	}
	
	mousedown_requested = this;
}

void LevelMakerObject::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	if (selecting)
		just_selected = true;
	
	// if clicked outside every object, request for deselection
	if ((!mouseInsideAny()) && (!mouseDownInsideAny()) && (!just_selected))
		deselection_requested = true;
	// if this object just was created by a panel
	else if (just_created)
		just_created = false;
	// if no drag was done, request toggle
	else if ((isSelected()) && (getShape()->mouseInside()) && (mouse_down_position == getShape()->position))
		toggle_requested = true;
	
	dragging = false;
	selecting = false;
}

void LevelMakerObject::handleKeyDown(const observer::Event& event, bool& stop) {
	if (((inputmanager_event.key.keysym.sym == SDLK_LCTRL) ||
		(inputmanager_event.key.keysym.sym == SDLK_RCTRL)) &&
		(!selecting)) {
		mouse_ctrl_position = r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
		mouse_down_position = getShape()->position;
		dragging = true;
	}
}

void LevelMakerObject::handleKeyUp(const observer::Event& event, bool& stop) {
	if ((inputmanager_event.key.keysym.sym == SDLK_LCTRL) ||
		(inputmanager_event.key.keysym.sym == SDLK_RCTRL))
		dragging = false;
}
