
#ifndef LEVELMAKERPANEL_HPP
#define LEVELMAKERPANEL_HPP

#include "observer.hpp"

#include "GameObject.hpp"
#include "LevelMakerObject.hpp"

class LevelMakerPanel : public GameObject {
private:
	static LevelMakerPanel* panels[LevelMakerObject::LASTTYPE];
	static LevelMakerPanel* current_panel;
	static bool clicked_outside;
	static bool hooked;
	static lalge::R2Vector diff_position;
protected:
	LevelMakerPanel();
	virtual ~LevelMakerPanel();
public:
	static void init();
	static void close();
	
	static void checkDeselectionRequest();
	static void updateCurrent();
private:
	virtual void show() = 0;
	virtual void hide() = 0;
	
	virtual void update();
protected:
	virtual void handleMouseDownLeft(const observer::Event& event, bool& stop);
	virtual void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
