#include "LevelMakerPanel.hpp"

#include "Rectangle.hpp"
#include "InputManager.hpp"
#include "PanelGeneral.hpp"

using namespace lalge;

Sprite* LevelMakerPanel::bg = 0;
LevelMakerData* LevelMakerPanel::data = 0;
LevelMakerPanel* LevelMakerPanel::panels[LevelMakerObject::LASTTYPE];
LevelMakerPanel* LevelMakerPanel::current_panel = 0;
bool LevelMakerPanel::hooked = false;
bool LevelMakerPanel::just_unhooked = false;
R2Vector LevelMakerPanel::mouse_down_position;
LevelMakerObject* LevelMakerPanel::creating = 0;

LevelMakerPanel::LevelMakerPanel() {
	setShape(new Rectangle());
	sprite = bg;
	setupRectangle();
	
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &LevelMakerPanel::handleMouseDownLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &LevelMakerPanel::handleMouseUpLeft);
}

LevelMakerPanel::~LevelMakerPanel() {
	InputManager::instance()->disconnect(this);
}

void LevelMakerPanel::setupRectangle() {
	((Rectangle*)getShape())->setWidth(sprite->rectW());
	((Rectangle*)getShape())->setHeight(sprite->rectH());
}

R2Vector LevelMakerPanel::getOrigin() const {
	return getShape()->position - r2vec(sprite->rectW(), sprite->rectH())/2;
}

void LevelMakerPanel::init(LevelMakerData* data) {
	if ((LevelMakerPanel::data) || (!data))
		return;
	
	bg = new Sprite("img/levelmaker/panel.png");
	
	LevelMakerPanel::data = data;
	
	panels[LevelMakerObject::NONE] = new PanelGeneral();
	panels[LevelMakerObject::AVATAR] = new PanelGeneral();
	panels[LevelMakerObject::BLACKHOLE] = new PanelGeneral();
	panels[LevelMakerObject::KEY] = new PanelGeneral();
	panels[LevelMakerObject::PARTICLE] = new PanelGeneral();
	panels[LevelMakerObject::ITEM] = new PanelGeneral();
	current_panel = panels[LevelMakerObject::NONE];
	current_panel->getShape()->position = r2vec(640, 360);
}

void LevelMakerPanel::close() {
	if (!LevelMakerPanel::data)
		return;
	
	data = 0;
	
	delete bg;
	bg = 0;
	
	for (int i = 0; i < LevelMakerObject::LASTTYPE; ++i)
		delete panels[i];
	current_panel = 0;
}

bool LevelMakerPanel::mouseInside() { return current_panel->getShape()->mouseInside(); }

void LevelMakerPanel::checkSelectionRequests() {
	if ((hooked) || (just_unhooked) || (creating)) {
		if (just_unhooked)
			just_unhooked = false;
		LevelMakerObject::deselection_requested = false;
		LevelMakerObject::selection_requested = false;
		LevelMakerObject::toggle_requested = false;
		LevelMakerObject::mousedown_requested = 0;
		return;
	}
	
	if (LevelMakerObject::deselection_requested) {
		LevelMakerObject::deselect();
	}
	else if (LevelMakerObject::selection_requested) {
		LevelMakerObject::deselect();
		LevelMakerObject::startSelection();
	}
	else if (LevelMakerObject::toggle_requested) {
		LevelMakerObject::toggle();
	}
	else if (LevelMakerObject::mousedown_requested) {
		LevelMakerObject::handleMouseDown();
	}
}

void LevelMakerPanel::updateCurrent() {
	R2Vector new_size;
	R2Vector old_size;
	LevelMakerPanel* old_panel = current_panel;
	current_panel = panels[LevelMakerObject::getSelectedType()];
	
	new_size = r2vec(((Rectangle*)current_panel->getShape())->getWidth(), ((Rectangle*)current_panel->getShape())->getHeight());
	old_size = r2vec(((Rectangle*)old_panel->getShape())->getWidth(), ((Rectangle*)old_panel->getShape())->getHeight());
	current_panel->getShape()->position = old_panel->getShape()->position + (new_size - old_size)/2;
	
	if (hooked)
		current_panel->getShape()->position = mouse_down_position + r2vec(InputManager::instance()->mouseDiffX(), InputManager::instance()->mouseDiffY());
	
	old_panel->hide();
	current_panel->show();
	
	current_panel->update();
	
	if (creating) {
		creating->getShape()->position = r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
		creating->getGameObject()->getShape()->position = creating->getShape()->position;
	}
}

void LevelMakerPanel::renderCurrent() {
	current_panel->sprite->render(current_panel->getShape()->position.x(0), current_panel->getShape()->position.x(1), true);
	current_panel->render();
	
	if (creating)
		creating->render();
}

void LevelMakerPanel::create() {
	switch (creating->getType()) {
	case LevelMakerObject::KEY:
		data->key = creating;
		break;
		
	case LevelMakerObject::PARTICLE:
		data->particles.push_back(creating);
		break;
		
	case LevelMakerObject::ITEM:
		data->items.push_back(creating);
		break;
		
	default:
		break;
	}
	
	LevelMakerObject::selected.clear();
	LevelMakerObject::selected.insert(creating);
	creating->just_created = true;
	
	creating = 0;
}

void LevelMakerPanel::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (creating)
		create();
	
	if ((getShape()->mouseDownInside()) && (this == current_panel)) {
		hooked = true;
		mouse_down_position = getShape()->position;
		just_unhooked = false;
	}
}

void LevelMakerPanel::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	if (hooked)
		just_unhooked = true;
	hooked = false;
}
