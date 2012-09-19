
#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#include "GameObject.hpp"

class Interaction {
public:
	typedef void (GameObject::*callback)(GameObject* target);
private:
	GameObject* go1;
	GameObject* go2;
	
	callback handler;
	
	bool enabled;
	bool twice;
public:
	Interaction(GameObject* go1, GameObject* go2, callback handler, bool twice = false);
	void interact();
	void enable(bool enable);
};

#endif
