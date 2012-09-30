
#ifndef PANELGENERAL_HPP
#define PANELGENERAL_HPP

#include "LevelMakerPanel.hpp"

class PanelGeneral : public LevelMakerPanel {
public:
	PanelGeneral();
	~PanelGeneral();
	
	void update();
private:
	void handleMouseDownLeft(const observer::Event& event, bool& stop);
};

#endif
