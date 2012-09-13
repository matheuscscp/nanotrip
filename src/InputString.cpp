#include "configfile.hpp"

#include "InputString.hpp"

#include "InputManager.hpp"

#define AVAILABLE_INPUT	\
"1234567890 abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

using std::vector;
using std::string;

vector< bool > InputString::available_input;

InputString::InputString(const string& str, bool flag) :
active(false), buf(str)
{
	if( !available_input.size() )
		initAvailableInput();
	
	subject.init( 2 );
	
	enable( flag );
}

InputString::~InputString()
{
	if( active )
		InputManager::instance()->disconnect( this );
}

void InputString::initAvailableInput()
{
	Configuration tmp;
	string stmp;
	
	available_input.resize( SDLK_LAST, false );
	
	try {
		tmp.readTxt( "conf/InputString.conf" );
		stmp = tmp.getStr( "available_input" );
		
		if( !stmp.size() )
			stmp = AVAILABLE_INPUT;
		
	} catch(Configuration::FileNotFound& e) {
		stmp = AVAILABLE_INPUT;
	} catch(Configuration::VarNotFound& e) {
		stmp = AVAILABLE_INPUT;
	}
	
	for( int i = 0; i < int( stmp.size() ); ++i )
		available_input[ int( stmp[i] ) ] = true;
}

void InputString::handleKeyDown(const observer::Event& event, bool& stop) {
	int tmp = inputmanager_event.key.keysym.sym;
	bool broadcast = true;
	
	if( ( tmp == SDLK_BACKSPACE ) && ( buf.size() ) )
		buf.erase( buf.size() - 1 );
	else if( ( tmp == SDLK_KP_ENTER ) || ( tmp == SDLK_RETURN ) )
		subject.broadcast( observer::Event( ENTER ) );
	else
	{
		if(	(	( inputmanager_event.key.keysym.sym >= SDLK_a ) &&
				( inputmanager_event.key.keysym.sym <= SDLK_z )	) &&
			(	( InputManager::instance()->keyPressed( SDLK_RSHIFT ) ) ||
				( InputManager::instance()->keyPressed( SDLK_LSHIFT ) )	)	)
		{
			tmp -= 32;
		}
		else if(	( inputmanager_event.key.keysym.sym >= SDLK_KP0 ) &&
					( inputmanager_event.key.keysym.sym <= SDLK_KP9 )	)
		{
			tmp += ( '0' - SDLK_KP0 );
		}
		
		if( available_input[tmp] )
			buf += char( tmp );
		else
			broadcast = false;
	}
	
	if( broadcast )
		subject.broadcast( observer::Event( UPDATE ) );
}

void InputString::enable(bool flag)
{
	if( ( !active ) && ( flag ) )
	{
		InputManager::instance()->connect(
			InputManager::KEYDOWN,
			this,
			&InputString::handleKeyDown
		);
		active = true;
	}
	else if( ( active ) && ( !flag ) )
	{
		InputManager::instance()->disconnect( this );
		active = false;
	}
}
