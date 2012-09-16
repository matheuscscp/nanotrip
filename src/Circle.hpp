
#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "Shape.hpp"

class Circle : public Shape {
private:
	lalge::Scalar radius;
public:
	Circle();
	virtual ~Circle();
	
	lalge::Scalar getRadius() const;
	void setRadius(lalge::Scalar radius);
	
	lalge::Scalar area() const;
	lalge::Scalar perimeter() const;
	
	bool collides(const Circle& target) const;
};

#endif
