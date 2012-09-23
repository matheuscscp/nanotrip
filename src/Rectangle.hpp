
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Shape.hpp"

class Rectangle : public Shape {
private:
	lalge::Scalar width;
	lalge::Scalar height;
public:
	Rectangle();
	virtual ~Rectangle();
	
	bool pointInside(int x, int y) const;
	
	lalge::Scalar getWidth() const;
	void setWidth(lalge::Scalar width);
	
	lalge::Scalar getHeight() const;
	void setHeight(lalge::Scalar height);
	
	lalge::Scalar area() const;
	lalge::Scalar perimeter() const;
};

#endif
