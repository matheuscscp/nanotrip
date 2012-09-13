#include <climits>

#include "InputManager.hpp"

#define broadcast(X)	subject.broadcast(Event((X), &event_))

InputManager* InputManager::instance_ = 0;

InputManager::Event::Event(int type, SDL_Event* event) : observer::Event(type), event(event) {}
InputManager::Event::~Event() {}

InputManager::InputManager() : mouse_x(INT_MAX), mouse_y(INT_MAX) {
	subject.init(LASTEVENT);
	
	for( int i = 0; i < SDLK_LAST; i++ )
		key_pressed[i] = false;
	
	for( int i = 0; i < INPUTMANAGER_BUTTONS; i++ )
		mouse_pressed[i] = false;
}

InputManager::~InputManager() {}

InputManager* InputManager::instance() {
	if (!instance_)
		instance_ = new InputManager();
	return instance_;
}

void InputManager::close() {
	if (instance_) {
		delete instance_;
		instance_ = 0;
	}
}

void InputManager::update()
{
	while( SDL_PollEvent( &event_ ) )
	{
		switch( event_.type )
		{
		case SDL_KEYDOWN:
			key_pressed[ event_.key.keysym.sym ] = true;
			broadcast ( KEYDOWN );
			break;
			
		case SDL_KEYUP:
			key_pressed[ event_.key.keysym.sym ] = false;
			broadcast ( KEYUP );
			break;
			
		case SDL_MOUSEMOTION:
			mouse_x = event_.motion.x;
			mouse_y = event_.motion.y;
			broadcast ( MOUSEMOTION );
			break;
			
		case SDL_MOUSEBUTTONDOWN:
			mouse_pressed[ event_.button.button ] = true;
			
			mousedown_x = event_.button.x;
			mousedown_y = event_.button.y;
			
			switch ( event_.button.button )
			{
			case SDL_BUTTON_LEFT:
				broadcast ( MOUSEDOWN_LEFT );
				break;
				
			case SDL_BUTTON_MIDDLE:
				broadcast ( MOUSEDOWN_MIDDLE );
				break;
				
			case SDL_BUTTON_RIGHT:
				broadcast ( MOUSEDOWN_RIGHT );
				break;
				
			case SDL_BUTTON_WHEELUP:
				broadcast ( MOUSE_WHEELUP );
				break;
				
			case SDL_BUTTON_WHEELDOWN:
				broadcast ( MOUSE_WHEELDOWN );
				break;
			}
			
			break;
			
		case SDL_MOUSEBUTTONUP:
			mouse_pressed[ event_.button.button ] = false;
			
			switch ( event_.button.button )
			{
			case SDL_BUTTON_LEFT:
				broadcast ( MOUSEUP_LEFT );
				break;
				
			case SDL_BUTTON_MIDDLE:
				broadcast ( MOUSEUP_MIDDLE );
				break;
				
			case SDL_BUTTON_RIGHT:
				broadcast ( MOUSEUP_RIGHT );
				break;
			}
			
			break;
			
		case SDL_QUIT:
			broadcast ( QUIT );
			break;
		}
	}
}

bool InputManager::keyPressed(int i) const { return key_pressed[i]; }
int InputManager::mouseX() const { return mouse_x; }
int InputManager::mouseY() const { return mouse_y; }
bool InputManager::mousePressed(int i) const { return mouse_pressed[i]; }
int InputManager::mouseDownX() const { return mousedown_x; }
int InputManager::mouseDownY() const { return mousedown_y; }
