
#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <list>

#include "Shape.hpp"
#include "Sprite.hpp"

class GameObject {
public:
	static std::list< std::list<GameObject*>* > all;
private:
	Shape* shape;
public:
	Sprite* sprite;
	bool hidden;
protected:
	bool die;
public:
	GameObject();
	virtual ~GameObject();
	
	bool mustdie() const;
	
	Shape* getShape() const;
	void setShape(Shape* shape);
	
	static void updateAll();
	static void renderAll();
	
	virtual void update();
	virtual void render();
};

#endif
