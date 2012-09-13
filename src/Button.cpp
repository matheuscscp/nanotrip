#include "Button.hpp"

#include "InputManager.hpp"

using namespace lalge;

Button::Button(const R2Vector& r, Sprite* sprite, bool flag) :
Rectangle( r, 1, sprite->srcW(), sprite->srcH() / 3 ),
clicked(false), active(false), sprite(sprite)
{
	subject.init( 1 );
	
	enable( flag );
}

Button::~Button()
{
	if( active )
		InputManager::instance()->disconnect( this );
}

void Button::render()
{
	if( active )
	{
		int clip_y;
		
		if( clicked )
		{
			clicked = false;
			clip_y = ( 2 * sprite->srcH() / 3 );
		}
		else if( !mouseInside() )
			clip_y = 0;
		else if( !InputManager::instance()->mousePressed( SDL_BUTTON_LEFT ) )
			clip_y = ( sprite->srcH() / 3 );
		else if( mouseDownInside() )
			clip_y = ( 2 * sprite->srcH() / 3 );
		else
			clip_y = 0;
		
		sprite->clip( 0, clip_y, sprite->srcW(), sprite->srcH() / 3 );
		sprite->render(
			r.x( 0 ) - sprite->srcW() / 2,
			r.x( 1 ) - sprite->srcH() / 6
		);
	}
}

void Button::enable(bool flag)
{
	if( ( !active ) && ( flag ) )
	{
		InputManager::instance ()->connect(
			InputManager::MOUSEDOWN_LEFT,
			this,
			&Button::handleMouseDownLeft
		);
		InputManager::instance ()->connect(
			InputManager::MOUSEUP_LEFT,
			this,
			&Button::handleMouseUpLeft
		);
		active = true;
	}
	else if( ( active ) && ( !flag ) )
	{
		InputManager::instance()->disconnect( this );
		active = false;
	}
}

void Button::handleMouseDownLeft(const observer::Event& event, bool& stop) {
	if ((mouseDownInside()) && (active))
		clicked = true;
}

void Button::handleMouseUpLeft(const observer::Event& event, bool& stop) {
	if ((mouseDownInside()) && (mouseInside()) && (active)) {
		clicked = true;
		subject.broadcast(observer::Event(CLICKED));
	}
}
