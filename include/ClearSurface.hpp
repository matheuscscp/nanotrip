#ifndef CLEARSURFACE
#define CLEARSURFACE

#include <list>

#include "Sprite.hpp"
#include "Text.hpp"

class ClearSurface
{
private:
	class Surface;
	
	std::list< Surface* > surfaces;
public:
	ClearSurface();
	~ClearSurface();
	
	void render(int x = 0, int y = 0);
	void render(Sprite* sprite, int x = 0, int y = 0);
	void render(Text* text, int x = 0, int y = 0);
	
	void clear();
};

#endif
