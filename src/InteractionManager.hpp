
#ifndef INTERACTIONMANAGER_HPP
#define INTERACTIONMANAGER_HPP

#include <list>

#include "GameObject.hpp"

class Interaction {
public:
	typedef void (GameObject::*callback)(GameObject* target);
private:
	GameObject* go1;
	GameObject* go2;
	
	callback handler;
	
	bool twice;
public:
	Interaction(GameObject* go1, GameObject* go2, callback handler, bool twice = true);
	bool operator==(const Interaction& param) const;
	bool operator!=(const Interaction& param) const;
	void interact();
};

class InteractionManager {
private:
	std::list<Interaction*> interactions;
	
	std::list<Interaction*>::iterator find(const Interaction& interaction);
public:
	void insert(Interaction* interaction);
	void erase(const Interaction& interaction);
	void update();
};

#endif
