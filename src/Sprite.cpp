/// @ingroup MOD_SPRITE
/// @file Sprite.cpp
/// @brief Implementations of all methods of the Sprite class
/// @author Matheus Pimenta

#include "Sprite.hpp"

#include "SDLBase.hpp"
#include "SurfaceManager.hpp"

using std::string;

std::list< std::list<Sprite*>* > Sprite::all;

Sprite::Sprite() : src(0), rotozoomed(0), angle_(0), zoomx(1), zoomy(1) {
	all.back()->push_back(this);
}

Sprite::Sprite(const string& filename) : rotozoomed(0), angle_(0), zoomx(1), zoomy(1) {
	all.back()->push_back(this);
	load_(filename);
}

Sprite::~Sprite() {
	all.back()->remove(this);
	unload();
}

void Sprite::load (const string& filename)
{
	unload ();
	
	load_ ( filename );
}

void Sprite::load_ (const string& filename)
{
	src = SurfaceManager::instance()->loadIMG( filename );
	
	srcrect_.x = 0;
	srcrect_.y = 0;
	
	srcrect_.w = src->w;
	srcrect_.h = src->h;
}

void Sprite::unload ()
{
	if ( src )
	{
		SurfaceManager::instance()->free( src );
		src = NULL;
		
		restore ();
	}
}

void Sprite::update() {}

void Sprite::clip(int x, int y, int w, int h)
{
	srcrect_.x = x;
	srcrect_.y = y;
	srcrect_.w = w;
	srcrect_.h = h;
	
	if ( rotozoomed )
		rotozoom ();
}

const SDL_Rect& Sprite::srcrect () const
{
	return srcrect_;
}

int Sprite::srcW () const
{
	return src->w;
}

int Sprite::srcH () const
{
	return src->h;
}

int Sprite::rectW () const
{
	return srcrect_.w;
}

int Sprite::rectH () const
{
	return srcrect_.h;
}

void Sprite::render (int x, int y, bool center, bool surface_manager) {
	// calculate x and y
	
	if (!center) {
		if (rotozoomed) {	// not center and rotozoomed
			x += (srcrect_.w - rotozoomed->w)/2;
			y += (srcrect_.h - rotozoomed->h)/2;
		}
	}
	else {
		if (!rotozoomed) {	// center but not rotozoomed
			x -= srcrect_.w/2;
			y -= srcrect_.h/2;
		}
		else {	// center and rotozoomed
			x -= rotozoomed->w/2;
			y -= rotozoomed->h/2;
		}
	}
	
	// render for surface manager
	if (surface_manager) {
		if (rotozoomed)
			SurfaceManager::instance()->clone(x, y, rotozoomed);
		else if (src)
			SurfaceManager::instance()->clone(x, y, src, &srcrect_);
	}
	// normal render
	else {
		SDL_Rect dstrect;
		dstrect.x = x;
		dstrect.y = y;
		if (rotozoomed)
			SDLBase::renderSurface(rotozoomed, NULL, &dstrect);
		else if (src)
			SDLBase::renderSurface(src, &srcrect_, &dstrect);
	}
}

void Sprite::rotozoom (float angle, float zoomx, float zoomy, bool force)
{
	if (	( force ) ||
			(	( angle != angle_ ) ||
				( zoomx != this->zoomx ) ||
				( zoomy != this->zoomy )	)	)
	{
		angle_ = angle;
		this->zoomx = zoomx;
		this->zoomy = zoomy;
		
		rotozoom ();
	}
}

void Sprite::rotozoom ()
{
	restore ();
	
	if (	( !srcrect_.x ) &&
			( !srcrect_.y ) &&
			( srcrect_.w == src->w ) &&
			( srcrect_.h == src->h )	)
	{
		rotozoomed = SDLBase::rotozoom ( src, angle_, zoomx, zoomy );
	}
	else
	{
		SDL_Surface* tmp = SurfaceManager::instance()->clip( src, &srcrect_ );
		rotozoomed = SurfaceManager::instance()->rotozoom(
			tmp, angle_, zoomx, zoomy
		);
		SurfaceManager::instance()->free( tmp );
	}
}

void Sprite::restore()
{
	if( rotozoomed )
	{
		SurfaceManager::instance()->free( rotozoomed );
		rotozoomed = NULL;
	}
}

float Sprite::angle () const
{
	return angle_;
}
