#include "GameObject.hpp"

using namespace lalge;

GameObject::GameObject (const R2Vector& r, const Scalar& depthconst) :
r ( r ), depthconst ( depthconst )
{
}

GameObject::~GameObject ()
{
}

R2Vector GameObject::range (const R2Vector& param) const
{
	return ( param - r );
}
