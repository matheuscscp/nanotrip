
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "linearalgebra.hpp"

class Shape {
public:
	lalge::R2Vector position;
	
	Shape();
	virtual ~Shape();
	
	lalge::R2Vector range(const Shape& target) const;
	
	virtual bool mouseInside() const = 0;
	virtual bool mouseDownInside() const = 0;
};

#endif
