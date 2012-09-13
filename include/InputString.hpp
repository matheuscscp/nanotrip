#ifndef INPUTSTRING_HPP
#define INPUTSTRING_HPP

#include <vector>
#include <string>

#include "mod/observer.hpp"

class InputString
{
SUBJECT
public:
	enum
	{
		UPDATE = 0,	// 1
		ENTER		// 2
	};
private:
	static std::vector< bool > available_input;
	
	bool active;
public:
	std::string buf;
	
	InputString(const std::string& str = "", bool flag = true);
	~InputString();
private:
	void initAvailableInput();
	void handleKeyDown(const observer::Event& event, bool& stop);
public:
	void enable(bool flag);
};

#endif
