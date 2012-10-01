
#ifndef PANELBLACKHOLE_HPP
#define PANELBLACKHOLE_HPP

#include "LevelMakerPanel.hpp"
#include "Button.hpp"
#include "InputString.hpp"
#include "Text.hpp"

class PanelBlackHole : public LevelMakerPanel {
private:
	Sprite* sprite_input_mass;
	Button* button_input_mass;
	lalge::R2Vector input_mass_position;
	bool invalid_input_mass;
	InputString input_mass;
	Text* text_input_mass;
public:
	PanelBlackHole();
	~PanelBlackHole();
	
	void show();
	void hide();
	
	void update();
	void render();
private:
	void updatePositions();
	
	void handleInputMass(const observer::Event& event, bool& stop);
	void handleInputMassButton(const observer::Event& event, bool& stop);
};

#endif
