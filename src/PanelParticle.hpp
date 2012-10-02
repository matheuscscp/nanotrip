
#ifndef PANELPARTICLE_HPP
#define PANELPARTICLE_HPP

#include "LevelMakerPanel.hpp"
#include "Button.hpp"
#include "InputString.hpp"
#include "Text.hpp"

class PanelParticle : public LevelMakerPanel {
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
	
	Sprite* sprite_input_charge;
	Button* button_input_charge;
	lalge::R2Vector input_charge_position;
	bool invalid_input_charge;
	InputString input_charge;
	Text* text_input_charge;
public:
	PanelParticle();
	~PanelParticle();
	
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
	void handleInputCharge(const observer::Event& event, bool& stop);
	void handleInputChargeButton(const observer::Event& event, bool& stop);
};

#endif
