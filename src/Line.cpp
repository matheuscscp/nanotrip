#include "Line.hpp"

using namespace lalge;

Line::Line() : direction(r2vec(1, 1)) {}
Line::~Line() {}

bool Line::pointInside(Scalar x, Scalar y) const {
	if (!direction.x(0))
		return (x == position.x(0));
	if (!direction.x(1))
		return (y == position.x(1));
	return ((x - position.x(0))/direction.x(0) == (y - position.x(1))/direction.x(1));
}

const R2Vector& Line::getDirection() const {
	return direction;
}

void Line::setDirection(const R2Vector& direction) {
	if (direction.size())
		this->direction = direction;
}

bool Line::collides(const Circle& circle) const {
	Scalar a = 1;
	Scalar b = -direction.x(1)/direction.x(0);
	Scalar c = position.x(0)*direction.x(1)/direction.x(0) - position.x(1);
	
	return (ABSVAL(a*circle.position.x(0) + b*circle.position.x(1) + c)/sqrt(a*a + b*b) > circle.getRadius());
}
