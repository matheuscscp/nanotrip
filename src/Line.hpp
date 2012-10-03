
#ifndef LINE_HPP
#define LINE_HPP

#include "Circle.hpp"

class Line : public Shape {
private:
	lalge::R2Vector direction;
public:
	Line();
	virtual ~Line();
	
	bool pointInside(lalge::Scalar x, lalge::Scalar y) const;
	
	const lalge::R2Vector& getDirection() const;
	void setDirection(const lalge::R2Vector& direction);
	
	bool collides(const Circle& circle) const;
};

#endif
