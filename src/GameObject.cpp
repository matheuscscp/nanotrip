#include "GameObject.hpp"

GameObject::GameObject() : shape(0), sprite(0), hidden(false) {}

GameObject::~GameObject() {
	if (shape)
		delete shape;
}

Shape* GameObject::getShape() const { return shape; }

void GameObject::setShape(Shape* shape) {
	if (this->shape)
		delete shape;
	this->shape = shape;
}

void GameObject::update() {}

void GameObject::render() {
	if ((sprite) && (!hidden))
		sprite->render(shape->position.x(0), shape->position.x(1), true);
}
