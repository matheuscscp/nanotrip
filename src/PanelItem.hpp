
#ifndef PANELITEM_HPP
#define PANELITEM_HPP

#include "LevelMakerPanel.hpp"
#include "Button.hpp"
#include "InputString.hpp"
#include "Text.hpp"

class PanelItem : public LevelMakerPanel {
private:
	Sprite* sprite_input_mass;
	Button* button_input_mass;
	lalge::R2Vector input_mass_position;
	bool invalid_input_mass;
	InputString input_mass;
	Text* text_input_mass;
	
	Sprite* sprite_input_elasticity;
	Button* button_input_elasticity;
	lalge::R2Vector input_elasticity_position;
	bool invalid_input_elasticity;
	InputString input_elasticity;
	Text* text_input_elasticity;
	
	Sprite* sprite_input_value;
	Button* button_input_value;
	lalge::R2Vector input_value_position;
	bool invalid_input_value;
	InputString input_value;
	Text* text_input_value;
	
	Button* button_time;
	lalge::R2Vector button_time_position;
	Button* button_point;
	lalge::R2Vector button_point_position;
	Button* button_life;
	lalge::R2Vector button_life_position;
	Button* button_mass;
	lalge::R2Vector button_mass_position;
	Button* button_barrier;
	lalge::R2Vector button_barrier_position;
public:
	PanelItem();
	~PanelItem();
	
	void show();
	void hide();
	
	void update();
	void render();
private:
	void updatePositions();
	
	void handleInputMass(const observer::Event& event, bool& stop);
	void handleInputMassButton(const observer::Event& event, bool& stop);
	void handleInputElasticity(const observer::Event& event, bool& stop);
	void handleInputElasticityButton(const observer::Event& event, bool& stop);
	void handleInputValue(const observer::Event& event, bool& stop);
	void handleInputValueButton(const observer::Event& event, bool& stop);
	
	void handleTime(const observer::Event& event, bool& stop);
	void handlePoint(const observer::Event& event, bool& stop);
	void handleLife(const observer::Event& event, bool& stop);
	void handleMass(const observer::Event& event, bool& stop);
	void handleBarrier(const observer::Event& event, bool& stop);
};

#endif
