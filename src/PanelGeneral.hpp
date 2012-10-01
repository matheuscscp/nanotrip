
#ifndef PANELGENERAL_HPP
#define PANELGENERAL_HPP

#include "LevelMakerPanel.hpp"
#include "Button.hpp"
#include "InputString.hpp"
#include "Text.hpp"

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
	
	Sprite* sprite_input_time;
	Button* button_input_time;
	lalge::R2Vector input_time_position;
	bool invalid_input_time;
	InputString input_time;
	Text* text_input_time;
	
	Sprite* sprite_input_charge;
	Button* button_input_charge;
	lalge::R2Vector input_charge_position;
	bool invalid_input_charge;
	InputString input_charge;
	Text* text_input_charge;
	
	Sprite* sprite_input_bgm;
	Button* button_input_bgm;
	lalge::R2Vector input_bgm_position;
	bool invalid_input_bgm;
	InputString input_bgm;
	Text* text_input_bgm;
public:
	PanelGeneral();
	~PanelGeneral();
	
	void show();
	void hide();
	
	void update();
	void render();
private:
	void updatePositions();
	
	void handleBorderTop(const observer::Event& event, bool& stop);
	void handleBorderRight(const observer::Event& event, bool& stop);
	void handleBorderBottom(const observer::Event& event, bool& stop);
	void handleBorderLeft(const observer::Event& event, bool& stop);
	
	void handleAvatar(const observer::Event& event, bool& stop);
	void handleBlackHole(const observer::Event& event, bool& stop);
	void handleKey(const observer::Event& event, bool& stop);
	void handleParticle(const observer::Event& event, bool& stop);
	void handleItem(const observer::Event& event, bool& stop);
	
	void handleInputTime(const observer::Event& event, bool& stop);
	void handleInputTimeButton(const observer::Event& event, bool& stop);
	void handleInputCharge(const observer::Event& event, bool& stop);
	void handleInputChargeButton(const observer::Event& event, bool& stop);
	void handleInputBGM(const observer::Event& event, bool& stop);
	void handleInputBGMButton(const observer::Event& event, bool& stop);
};

#endif
