
#include "mod/common.hpp"

#include "StateManager.hpp"

using namespace common;

int main(int argc, char** argv) {
	MainArgs::init(argc, argv);
	StateManager sm;
	
	try {
		sm.run();
	} catch (Quit&) {
		// just quit
	} catch (mexception& e) {
		e.logerr();	// log the error
	}
	
	return 0;
}
