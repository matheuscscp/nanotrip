#include "LevelMakerPanel.hpp"

#include "Rectangle.hpp"
#include "InputManager.hpp"
#include "PanelGeneral.hpp"

using namespace lalge;

LevelMakerPanel* LevelMakerPanel::panels[LevelMakerObject::LASTTYPE];
LevelMakerPanel* LevelMakerPanel::current_panel = 0;
bool LevelMakerPanel::hooked = false;
R2Vector LevelMakerPanel::mouse_down_position;

LevelMakerPanel::LevelMakerPanel() {
	setShape(new Rectangle());
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &LevelMakerPanel::handleMouseDownLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &LevelMakerPanel::handleMouseUpLeft);
}

LevelMakerPanel::~LevelMakerPanel() {
	InputManager::instance()->disconnect(this);
}

void LevelMakerPanel::init() {
	if (current_panel)
		return;
	
	panels[LevelMakerObject::NONE] = new PanelGeneral();
	panels[LevelMakerObject::AVATAR] = new PanelGeneral();
	panels[LevelMakerObject::BLACKHOLE] = new PanelGeneral();
	panels[LevelMakerObject::KEY] = new PanelGeneral();
	panels[LevelMakerObject::PARTICLE] = new PanelGeneral();
	panels[LevelMakerObject::ITEM] = new PanelGeneral();
	current_panel = panels[LevelMakerObject::NONE];
}

void LevelMakerPanel::close() {
	if (!current_panel)
		return;
	
	for (int i = 0; i < LevelMakerObject::LASTTYPE; ++i)
		delete panels[i];
	current_panel = 0;
}

void LevelMakerPanel::checkSelectionRequests() {
	if (hooked) {
		LevelMakerObject::deselection_requested = false;
		LevelMakerObject::selection_requested = false;
		return;
	}
	
	if (LevelMakerObject::deselection_requested)
		LevelMakerObject::deselect();
	else if (LevelMakerObject::selection_requested) {
		LevelMakerObject::deselect();
		LevelMakerObject::startSelection();
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
	
	current_panel->update();
}

void LevelMakerPanel::renderCurrent() {
	current_panel->render();
}

void LevelMakerPanel::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (getShape()->mouseDownInside()) {
		hooked = true;
		mouse_down_position = getShape()->position;
	}
}

void LevelMakerPanel::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	hooked = false;
}
