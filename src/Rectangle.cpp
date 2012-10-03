#include "Rectangle.hpp"

using namespace lalge;

Rectangle::Rectangle() : width(1), height(1) {}
Rectangle::~Rectangle() {}

bool Rectangle::pointInside(Scalar x, Scalar y) const {
	if ((x >= position.x(0) - width/2) && (x < position.x(0) + width/2)) {
		return ((y >= position.x(1) - height/2) && (y < position.x(1) + height/2));
	}
	return false;
}

Scalar Rectangle::getWidth() const { return width; }
void Rectangle::setWidth(Scalar width) { if (width >= 1) this->width = width; }

Scalar Rectangle::getHeight() const { return height; }
void Rectangle::setHeight(Scalar height) { if (height >= 1) this->height = height; }

Scalar Rectangle::area() const { return width*height; }
Scalar Rectangle::perimeter() const { return 2*width + 2*height; }
