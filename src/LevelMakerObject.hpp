
#ifndef LEVELMAKEROBJECT_HPP
#define LEVELMAKEROBJECT_HPP

#include <set>

#include "observer.hpp"

#include "GameObject.hpp"
#include "Rectangle.hpp"

class LevelMakerObject : public GameObject {
public:
	enum {
		NONE = 0,
		AVATAR,
		BLACKHOLE,
		KEY,
		PARTICLE,
		ITEM,
		
		// don't change this
		LASTTYPE
	};
	
	static std::set<LevelMakerObject*> selected;
	static bool deselection_requested;
	static bool selection_requested;
	static bool toggle_requested;
	static Sprite* sprite_selection_box;
	static Sprite* sprite_selection_horizontal;
	static Sprite* sprite_selection_vertical;
private:
	static std::set<LevelMakerObject*> all;
	static bool dragging;
	static bool selecting;
	static bool just_selected;
	static bool selected_on_click;
	static Rectangle selection_box;
	
	lalge::R2Vector mouse_down_position;
	
	int type;
	GameObject* object;
public:
	Sprite* selection;
	
	LevelMakerObject(int type, GameObject* object);
	~LevelMakerObject();
	
	static void updateSelected();
	static void updateSelection();
	static void renderSelection();
	
	static int getSelectedType();
	
	static void deselect();
	static void startSelection();
	static void toggle();
private:
	void update();
public:
	void render();
	
	LevelMakerObject* clone();
	
	bool isSelected() const;
	
	int getType() const;
	GameObject* getGameObject() const;
private:
	static bool mouseInsideAny();
	static bool mouseDownInsideAny();
	
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
	void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
