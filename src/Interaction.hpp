
#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#include <list>

#include "GameObject.hpp"

class Interaction {
public:
	typedef void (GameObject::*callback)(GameObject* target);
	
	static std::list< std::list<Interaction*>* > all;
private:
	GameObject* go1;
	GameObject* go2;
	
	callback handler;
	
	bool enabled;
	bool twice;
	
	Interaction(GameObject* go1, GameObject* go2, callback handler, bool twice = false);
public:
	~Interaction();
private:
	static std::list<Interaction*>::iterator find(const Interaction& interaction);
	bool operator!=(const Interaction& param) const;
public:
	static Interaction* insert(GameObject* go1, GameObject* go2, callback handler, bool twice = false);
	static bool erase(GameObject* go1, GameObject* go2, callback handler);
	static bool erase(Interaction* interaction);
	
	static void interactAll();
	
	void enable(bool enable);
};

#endif
