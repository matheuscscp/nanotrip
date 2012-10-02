
#ifndef STATELEVELMAKER_HPP
#define STATELEVELMAKER_HPP

#include "State.hpp"
#include "LevelMakerData.hpp"
#include "Button.hpp"

class StateLevelMaker : public State {
GAMESTATE
public:
	class UnstackArgs : public ArgsBase {
	public:
		enum {
			SAVE_MENU,
			SAVE_QUIT,
			MENU,
			QUIT
		};
		
		int op;
		UnstackArgs(int op);
	};
	
	class Args : public ArgsBase {
	public:
		std::string levelname;
		Args(const std::string& levelname);
	};
private:
	int bg_x, bg_y;
	Sprite* bg;
	
	Sprite* hud;
	
	Sprite* border_top;
	Sprite* border_right;
	Sprite* border_bottom;
	Sprite* border_left;
	
	Button* button_save;
	Button* button_revert;
	Button* button_test;
	Button* button_quit;
	Button* button_delete;
	Button* button_clone;
	
	bool clicking_button;
	
	LevelMakerData* data;
public:
	StateLevelMaker(ArgsBase* args);
	~StateLevelMaker();
	
	void handleUnstack(ArgsBase* args);
	
	void update();
	void render();
private:
	void renderAvatarSpeed();
	
	void disableButtons();
	void enableButtons();
	
	void handleSave(const observer::Event& event, bool& stop);
	void handleRevert(const observer::Event& event, bool& stop);
	void handleTest(const observer::Event& event, bool& stop);
	void handleQuitButton(const observer::Event& event, bool& stop);
	void handleDelete(const observer::Event& event, bool& stop);
	void handleClone(const observer::Event& event, bool& stop);
	
	void handleKeyDown(const observer::Event& event, bool& stop);
	
	void handleQuit(const observer::Event& event, bool& stop);
	
	void checkSelectionRequests();
	
	void handleMouseLeft(const observer::Event& event, bool& stop);
};

#endif
