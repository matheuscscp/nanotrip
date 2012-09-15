#include "GameObject.hpp"

using std::list;

list< list<GameObject*>* > GameObject::all;

GameObject::GameObject() : shape(0), sprite(0), hidden(false), die(false) { all.back()->push_back(this); }

GameObject::~GameObject() {
	// not removed from GameObject::all if the object requested to die itself
	if (!die)
		all.back()->remove(this);
	
	// shape
	if (shape)
		delete shape;
}

bool GameObject::mustdie() const { return die; }

Shape* GameObject::getShape() const { return shape; }

void GameObject::setShape(Shape* shape) {
	if (shape)
		delete shape;
	this->shape = shape;
}

void GameObject::update() {}

void GameObject::render() {
	if ((sprite) && (!hidden))
		sprite->render(shape->position.x(0), shape->position.x(1), true);
}
