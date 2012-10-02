
#ifndef PANELAVATAR_HPP
#define PANELAVATAR_HPP

#include "LevelMakerPanel.hpp"
#include "Button.hpp"
#include "InputString.hpp"
#include "Text.hpp"

class PanelAvatar : public LevelMakerPanel {
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
	
	Button* set_speed;
	lalge::R2Vector set_speed_position;
public:
	PanelAvatar();
	~PanelAvatar();
	
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
	
	void handleSetSpeed(const observer::Event& event, bool& stop);
};

#endif
