#include "Shape.hpp"

Shape::Shape() {}
Shape::~Shape() {}

lalge::R2Vector Shape::range(const Shape& target) const {
	return (target.position - position);
}
