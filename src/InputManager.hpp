#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "SDL.h"

#include "observer.hpp"

#define inputmanager_event	(*(((InputManager::Event*)&event)->event))

#define INPUTMANAGER_BUTTONS	6

class InputManager {
SUBJECT
public:
	enum {
		// events
		QUIT = 0,
		KEYDOWN,
		KEYUP,
		MOUSEMOTION,
		MOUSEDOWN_LEFT,
		MOUSEDOWN_MIDDLE,
		MOUSEDOWN_RIGHT,
		MOUSEUP_LEFT,
		MOUSEUP_MIDDLE,
		MOUSEUP_RIGHT,
		MOUSE_WHEELUP,
		MOUSE_WHEELDOWN,
		
		// don't change this
		LASTEVENT
	};
	
	struct Event : public observer::Event {
		SDL_Event* event;
		
		Event(int type, SDL_Event* event);
		~Event();
	};
private:
	static InputManager* instance_;
	
	SDL_Event event_;
	
	bool key_pressed[SDLK_LAST];
	int mouse_x;
	int mouse_y;
	bool mouse_pressed[INPUTMANAGER_BUTTONS];
	int mousedown_x;
	int mousedown_y;
	
	InputManager ();
	~InputManager ();
public:
	static InputManager* instance();
	static void close();
	
	void update ();
	
	bool keyPressed (int i) const;
	int mouseX () const;
	int mouseY () const;
	bool mousePressed (int i) const;
	int mouseDownX () const;
	int mouseDownY () const;
};

#endif
