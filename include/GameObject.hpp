#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "mod/linearalgebra.hpp"

class GameObject
{
public:
	lalge::R2Vector r;
	
	lalge::Scalar depthconst;
	
	GameObject (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1
	);
	virtual ~GameObject ();
	
	virtual void update () = 0;
	
	virtual void render () = 0;
	
	virtual GameObject* clone () = 0;
	
	lalge::R2Vector range (const lalge::R2Vector& param) const;
};

#endif
