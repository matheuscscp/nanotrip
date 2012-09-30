#include "LevelMakerPanel.hpp"

#include "Rectangle.hpp"
#include "InputManager.hpp"
#include "PanelGeneral.hpp"

using namespace lalge;

LevelMakerPanel* LevelMakerPanel::panels[LevelMakerObject::LASTTYPE];
LevelMakerPanel* LevelMakerPanel::current_panel = 0;
bool LevelMakerPanel::clicked_outside = false;
bool LevelMakerPanel::hooked = false;
R2Vector LevelMakerPanel::diff_position;

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

void LevelMakerPanel::checkDeselectionRequest() {
	if (LevelMakerObject::deselection_requested)
		LevelMakerObject::startSelection();
}

void LevelMakerPanel::updateCurrent() {
	R2Vector new_size;
	R2Vector old_size;
	LevelMakerPanel* old_panel = current_panel;
	current_panel = panels[LevelMakerObject::getSelectedType()];
	
	new_size = r2vec(((Rectangle*)current_panel->getShape())->getWidth(), ((Rectangle*)current_panel->getShape())->getHeight());
	old_size = r2vec(((Rectangle*)old_panel->getShape())->getWidth(), ((Rectangle*)old_panel->getShape())->getHeight());
	current_panel->getShape()->position = old_panel->getShape()->position + (new_size - old_size)/2;
	
	current_panel->update();
}

void LevelMakerPanel::update() {
	if (hooked)
		getShape()->position = diff_position + r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
}

void LevelMakerPanel::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (getShape()->mouseDownInside()) {
		hooked = true;
		diff_position = getShape()->position - r2vec(InputManager::instance()->mouseDownX(), InputManager::instance()->mouseDownY());
	}
}

void LevelMakerPanel::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	hooked = false;
}
