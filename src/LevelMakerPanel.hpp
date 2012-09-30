
#ifndef LEVELMAKERPANEL_HPP
#define LEVELMAKERPANEL_HPP

#include "observer.hpp"

#include "GameObject.hpp"

class LevelMakerPanel : public GameObject {
private:
	lalge::R2Vector diff_position;
protected:
	bool hooked;
public:
	LevelMakerPanel();
	virtual ~LevelMakerPanel();
	
	virtual void update();
	
	bool isHooked() const;
protected:
	virtual void handleMouseDownLeft(const observer::Event& event, bool& stop);
private:
	void handleMouseUpLeft(const observer::Event& event, bool& stop);
};

#endif
