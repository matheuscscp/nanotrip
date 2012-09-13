#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "SDL.h"

#include "mod/observer.hpp"

#define inputmanager_event	(*(((InputManager::Event*)&event)->event))

#define INPUTMANAGER_N_EVENTS	12
#define INPUTMANAGER_KEYS		SDLK_LAST
#define INPUTMANAGER_BUTTONS	6

class InputManager
{
SUBJECT
public:
	enum {
		KEYDOWN = 0,		// 1
		KEYUP,				// 2
		MOUSEMOTION,		// 3
		MOUSEDOWN_LEFT,		// 4
		MOUSEDOWN_MIDDLE,	// 5
		MOUSEDOWN_RIGHT,	// 6
		MOUSEUP_LEFT,		// 7
		MOUSEUP_MIDDLE,		// 8
		MOUSEUP_RIGHT,		// 9
		MOUSE_WHEELUP,		// 10
		MOUSE_WHEELDOWN,	// 11
		QUIT				// 12
	};
	
	struct Event : public observer::Event
	{
		SDL_Event* event;
		
		Event(int type, SDL_Event* event);
		~Event();
	};
private:
	static InputManager* instance_;
	
	SDL_Event event_;
	
	bool key_pressed[INPUTMANAGER_KEYS];
	int mouse_x;
	int mouse_y;
	bool mouse_pressed[INPUTMANAGER_BUTTONS];
	int mousedown_x;
	int mousedown_y;
	
	InputManager ();
	~InputManager ();
public:
	static InputManager* instance ();
	static void close ();
	
	void update ();
	
	bool keyPressed (int i) const;
	int mouseX () const;
	int mouseY () const;
	bool mousePressed (int i) const;
	int mouseDownX () const;
	int mouseDownY () const;
};

#endif
