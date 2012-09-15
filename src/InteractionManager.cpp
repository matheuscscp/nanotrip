#include "InteractionManager.hpp"

#include "common.hpp"

using std::list;

Interaction::Interaction(GameObject* go1, GameObject* go2, callback handler, bool twice) :
go1(go1), go2(go2), handler(handler), twice(twice) {}

void Interaction::interact() {
	CALLBACK(*go1, handler)(go2);
	if (twice)
		CALLBACK(*go2, handler)(go1);
}

bool Interaction::operator==(const Interaction& param) const {
	return ((go1 == param.go1) && (go2 == param.go2) && (handler == param.handler));
}

bool Interaction::operator!=(const Interaction& param) const {
	return ((go1 != param.go1) || (go2 != param.go2) || (handler != param.handler));
}

list<Interaction*>::iterator InteractionManager::find(const Interaction& interaction) {
	list<Interaction*>::iterator it = interactions.begin();
	while ((it != interactions.end()) && (*(*it) != interaction))
		++it;
	return it;
}

void InteractionManager::insert(Interaction* interaction) {
	if (find(*interaction) == interactions.end())
		interactions.push_back(interaction);
}

void InteractionManager::erase(const Interaction& interaction) {
	list<Interaction*>::iterator it = find(interaction);
	if (it != interactions.end())
		interactions.erase(it);
}
void InteractionManager::update() {
	for (list<Interaction*>::iterator it = interactions.begin(); it != interactions.end(); ++it)
		(*it)->interact();
}
