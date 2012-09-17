#include "InputString.hpp"

#include "configfile.hpp"
#include "common.hpp"

#include "InputManager.hpp"

#define AVAILABLE_INPUT	\
"1234567890 abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

using std::vector;
using std::string;

vector< bool > InputString::available_input;

InputString::InputString(const string& str) : buf(str), max_size(0) {
	if (!available_input.size())
		initAvailableInput();
	subject.init(LASTEVENT);
	InputManager::instance()->connect(InputManager::KEYDOWN, this, &InputString::handleKeyDown);
	InputManager::instance()->connect(InputManager::KEYUP, this, &InputString::handleKeyUp);
}

InputString::~InputString() {
	InputManager::instance()->disconnect(this);
}

int InputString::getMaxSize() const { return max_size; }

void InputString::setMaxSize(int max_size) {
	if (max_size < 0)
		return;
	this->max_size = max_size;
	if ((max_size) && (buf.size() > max_size))
		buf.erase(max_size);
}

const string& InputString::get() const { return buf; }

void InputString::set(const std::string& input) {
	buf = input;
	subject.broadcast(observer::Event(UPDATE));
}

void InputString::update() {
	if ((InputManager::instance()->keyPressed(SDLK_BACKSPACE)) &&
		(!timer.ispaused()) && (timer.time() >= 500) && (buf.size())) {
		buf.erase(buf.size() - 1);
		subject.broadcast(observer::Event(UPDATE));
	}
}

void InputString::initAvailableInput()
{
	Configuration tmp;
	string stmp;
	
	available_input.resize( SDLK_LAST, false );
	
	try {
		tmp.readTxt(common::RootPath::get("conf/InputString.conf"));
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
	
	if( ( tmp == SDLK_BACKSPACE ) && ( buf.size() ) ) {
		buf.erase( buf.size() - 1 );
		timer.start();
	}
	else if( ( tmp == SDLK_KP_ENTER ) || ( tmp == SDLK_RETURN ) ) {
		subject.broadcast( observer::Event( ENTER ) );
		broadcast = false;
	}
	else if ((max_size) && (buf.size() == max_size))
		broadcast = false;
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

void InputString::handleKeyUp(const observer::Event& event, bool& stop) {
	if (inputmanager_event.key.keysym.sym == SDLK_BACKSPACE)
		timer.pause();
}
