#include "Interaction.hpp"

#include "common.hpp"

using std::list;

std::list< std::list<Interaction*>* > Interaction::all;

Interaction::Interaction(GameObject* go1, GameObject* go2, callback handler, bool twice) :
go1(go1), go2(go2), handler(handler), enabled(true), twice(twice) {}
Interaction::~Interaction() { all.back()->remove(this); }

list<Interaction*>::iterator Interaction::find(const Interaction& interaction) {
	list<Interaction*>::iterator it = all.back()->begin();
	while ((it != all.back()->end()) && (*(*it) != interaction))
		++it;
	return it;
}

bool Interaction::operator!=(const Interaction& param) const {
	return ((go1 != param.go1) || (go2 != param.go2) || (handler != param.handler));
}

Interaction* Interaction::insert(GameObject* go1, GameObject* go2, callback handler, bool twice) {
	Interaction* interaction = new Interaction(go1, go2, handler, twice);
	list<Interaction*>::iterator it = find(*interaction);
	if (it == all.back()->end())
		all.back()->push_back(interaction);
	else {
		delete interaction;
		interaction = *it;
	}
	return interaction;
}

bool Interaction::erase(GameObject* go1, GameObject* go2, callback handler) {
	Interaction* interaction = new Interaction(go1, go2, handler);
	bool found = erase(interaction);
	delete interaction;
	return found;
}

bool Interaction::erase(Interaction* interaction) {
	list<Interaction*>::iterator it = find(*interaction);
	bool found = (it != all.back()->end());
	if (found)
		delete *it;
	return found;
}

void Interaction::interactAll() {
	for (list<Interaction*>::iterator it = all.back()->begin(); it != all.back()->end(); ++it) {
		if ((*it)->enabled) {
			CALLBACK(*(*it)->go1, (*it)->handler)((*it)->go2);
			if ((*it)->twice)
				CALLBACK(*(*it)->go2, (*it)->handler)((*it)->go1);
		}
	}
	
}

void Interaction::enable(bool enable) {
	enabled = enable;
}
