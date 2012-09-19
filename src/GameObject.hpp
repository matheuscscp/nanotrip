
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Shape.hpp"
#include "Sprite.hpp"

class GameObject {
private:
	Shape* shape;
public:
	Sprite* sprite;
	bool hidden;
	
	GameObject();
	virtual ~GameObject();
	
	Shape* getShape() const;
	void setShape(Shape* shape);
	
	static void updateAll();
	static void renderAll();
	
	virtual void update();
	virtual void render();
};

#endif
