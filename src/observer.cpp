#include "observer.hpp"

#include "common.hpp"

using namespace observer;

using std::list;

// =============================================================================
// Static vars
// =============================================================================

bool Stack::is_init = false;
int Stack::id_ = 0;
Subject Stack::subject;

// =============================================================================
// Event Class
// =============================================================================

Event::Event(int type) : type_(type) {}
Event::~Event() {}
int Event::type() const { return type_; }

// =============================================================================
// Observer Class
// =============================================================================

Subject::Observer::Observer() : stack_id(Stack::id()) {}
Subject::Observer::~Observer() {}

// =============================================================================
// Subject Class
// =============================================================================

Subject::Subject() : n_events(0), observers(0), broadcasting(0) {}

Subject::Subject(const Subject& rv) : n_events(0), observers(0), broadcasting(0) {
	copy(rv);
}

Subject::~Subject() {
	clear();
}

Subject& Subject::operator=(const Subject& rv) {
	clear();
	copy(rv);
	return *this;
}

void Subject::copy(const Subject& rv) {
	if (!rv.n_events)
		return;
	
	init(rv.n_events);
	
	// copy all observers of all events
	for (int i = 0; i < n_events; ++i) {
		// copy all observers
		for (list<Observer*>::iterator it = rv.observers[i].begin(); it != rv.observers[i].end(); ++it)
			observers[i].push_back((*it)->clone());
	}
}

void Subject::clear() {
	if (!n_events)
		return;
	
	for (int i = 0; i < n_events; ++i) {
		// dumps all observers
		while (observers[i].size()) {
			delete observers[i].back();
			observers[i].pop_back();
		}
	}
	delete[] observers;
	delete[] broadcasting;
	n_events = 0;
	observers = 0;
	broadcasting = 0;
}

void Subject::init(int n_events) {
	if ((this->n_events) || (!n_events))
		return;
	
	this->n_events = n_events;
	observers = new list<Observer*>[n_events];
	broadcasting = new bool[n_events];
	
	for (int i = 0; i < n_events; ++i)
		broadcasting[i] = false;
}

void Subject::broadcast(const Event& event) {
	int event_type = event.type();
	
	if (broadcasting[event_type])
		return;
	
	broadcasting[event_type] = true;
	
	bool stop = false;
	for (list<Observer*>::iterator it = observers[event_type].begin(); (it != observers[event_type].end()) && (!stop); ++it) {
		// the callback is done only for the observers in the top of the stack
		if ((*it)->stack_id == Stack::id()) {
			try {
				(*it)->callHandler(event, stop);
			} catch (common::mexception* e) {
				broadcasting[event_type] = false;
				throw e;
			}
		}
	}
	
	broadcasting[event_type] = false;
}

// =============================================================================
// Stack Class
// =============================================================================

bool Stack::init() {
	if (!is_init) {
		is_init = true;
		subject.init(2);
		return true;
	}
	return false;
}

int Stack::id() {
	return id_;
}

void Stack::push() {
	init();
	subject.broadcast(Event(PUSH));
	++id_;
}

void Stack::pop() {
	if (!init()) {
		--id_;
		subject.broadcast(Event(POP));
	}
}
