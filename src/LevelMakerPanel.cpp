#include "LevelMakerPanel.hpp"

#include "Rectangle.hpp"
#include "InputManager.hpp"

using namespace lalge;

LevelMakerPanel::LevelMakerPanel() : hooked(false) {
	setShape(new Rectangle());
	InputManager::instance()->connect(InputManager::MOUSEDOWN_LEFT, this, &LevelMakerPanel::handleMouseDownLeft);
	InputManager::instance()->connect(InputManager::MOUSEUP_LEFT, this, &LevelMakerPanel::handleMouseUpLeft);
}

LevelMakerPanel::~LevelMakerPanel() {
	InputManager::instance()->disconnect(this);
}

void LevelMakerPanel::update() {
	if (hooked)
		getShape()->position = diff_position + r2vec(InputManager::instance()->mouseX(), InputManager::instance()->mouseY());
}

bool LevelMakerPanel::isHooked() const { return hooked; }

void LevelMakerPanel::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if (getShape()->mouseDownInside()) {
		hooked = true;
		diff_position = getShape()->position - r2vec(InputManager::instance()->mouseDownX(), InputManager::instance()->mouseDownY());
	}
}

void LevelMakerPanel::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	hooked = false;
}
