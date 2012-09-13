#include "ClearSurface.hpp"

#include "SurfaceManager.hpp"
#include "SDLBase.hpp"

using std::list;

class ClearSurface::Surface
{
private:
	int x;
	int y;
	
	SDL_Surface* surface;
public:
	Surface();
	~Surface();
	
	void render(int x = 0, int y = 0);
};

ClearSurface::Surface::Surface()
{
	surface = SurfaceManager::instance()->getCloned( x, y );
}

ClearSurface::Surface::~Surface()
{
	SurfaceManager::instance()->free( surface );
}

void ClearSurface::Surface::render(int x, int y)
{
	SDL_Rect dstrect;
	
	dstrect.x = x + this->x;
	dstrect.y = y + this->y;
	
	SDLBase::renderSurface( surface, NULL, &dstrect );
}

ClearSurface::ClearSurface()
{
}

ClearSurface::~ClearSurface()
{
	clear();
}

void ClearSurface::render(int x, int y)
{
	for(
		list< Surface* >::iterator it = surfaces.begin();
		it != surfaces.end();
		++it
	)
	{
		(*it)->render( x, y );
	}
}

void ClearSurface::render(Sprite* sprite, int x, int y)
{
	sprite->render( x, y, true );
	surfaces.push_back( new Surface() );
}

void ClearSurface::render(Text* text, int x, int y)
{
	text->render( x, y, true );
	surfaces.push_back( new Surface() );
}

void ClearSurface::clear()
{
	while( surfaces.size() )
	{
		delete surfaces.back();
		surfaces.pop_back();
	}
}
