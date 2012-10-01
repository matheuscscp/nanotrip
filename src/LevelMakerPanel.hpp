
#ifndef LEVELMAKERPANEL_HPP
#define LEVELMAKERPANEL_HPP

#include "observer.hpp"

#include "GameObject.hpp"
#include "LevelMakerData.hpp"

class LevelMakerPanel : public GameObject {
private:
	static Sprite* bg;
protected:
	static LevelMakerData* data;
private:
	static LevelMakerPanel* panels[LevelMakerObject::LASTTYPE];
	static LevelMakerPanel* current_panel;
	static bool hooked;
	static bool just_unhooked;
	static lalge::R2Vector mouse_down_position;
public:
	static LevelMakerObject* creating;
protected:
	LevelMakerPanel();
	virtual ~LevelMakerPanel();
	
	void setupRectangle();
	
	lalge::R2Vector getOrigin() const;
public:
	static void init(LevelMakerData* data);
	static void close();
	
	static bool mouseInside();
	
	static void checkSelectionRequests();
	static void updateCurrent();
	static void renderCurrent();
private:
	static void create();
	
	virtual void show() = 0;
	virtual void hide() = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
	void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
