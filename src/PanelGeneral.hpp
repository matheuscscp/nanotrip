
#ifndef PANELGENERAL_HPP
#define PANELGENERAL_HPP

#include "LevelMakerPanel.hpp"
#include "Button.hpp"

class PanelGeneral : public LevelMakerPanel {
private:
	Button* border_top;
	lalge::R2Vector border_top_position;
	Button* border_right;
	lalge::R2Vector border_right_position;
	Button* border_bottom;
	lalge::R2Vector border_bottom_position;
	Button* border_left;
	lalge::R2Vector border_left_position;
	
	Button* avatar;
	lalge::R2Vector avatar_position;
	Button* blackhole;
	lalge::R2Vector blackhole_position;
	Button* key;
	lalge::R2Vector key_position;
	Button* particle;
	lalge::R2Vector particle_position;
	Button* item;
	lalge::R2Vector item_position;
public:
	PanelGeneral();
	~PanelGeneral();
	
	void show();
	void hide();
	
	void update();
	void render();
private:
	void updateBordersPosition();
	
	void handleBorderTop(const observer::Event& event, bool& stop);
	void handleBorderRight(const observer::Event& event, bool& stop);
	void handleBorderBottom(const observer::Event& event, bool& stop);
	void handleBorderLeft(const observer::Event& event, bool& stop);
};

#endif
