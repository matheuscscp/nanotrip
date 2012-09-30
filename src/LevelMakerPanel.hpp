
#ifndef LEVELMAKERPANEL_HPP
#define LEVELMAKERPANEL_HPP

#include "observer.hpp"

#include "GameObject.hpp"
#include "LevelMakerObject.hpp"

class LevelMakerPanel : public GameObject {
private:
	static LevelMakerPanel* panels[LevelMakerObject::LASTTYPE];
	static LevelMakerPanel* current_panel;
	static bool hooked;
	static lalge::R2Vector mouse_down_position;
protected:
	LevelMakerPanel();
	virtual ~LevelMakerPanel();
public:
	static void init();
	static void close();
	
	static void checkSelectionRequests();
	static void updateCurrent();
	static void renderCurrent();
private:
	virtual void show() = 0;
	virtual void hide() = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
	void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
