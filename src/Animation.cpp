#include "common.hpp"

#include "Animation.hpp"

#include "SDLBase.hpp"

using std::string;

Animation::Animation (
	const string& filename,
	int frame,
	int framesize,
	int rows,
	int cols,
	bool matrix,
	int line
) : rows ( rows ), cols ( cols ), matrix ( matrix ), line ( line )
{
	load ( filename );
	
	if ( ( matrix ) && ( frame >= cols ) )
		frame = 0;
	else
		this->frame = float ( frame );
	setFrameSize ( framesize );
	
	update_ ();
}

void Animation::update ()
{
	float dt = float ( SDLBase::dt () ) / 1000;
	
	frame = ( ( frame + fps * dt ) - float (
		( int ( frame + fps * dt ) / frameAmount() ) * frameAmount()
	) );
	
	update_ ();
}

void Animation::update_ ()
{
	clip (
		( int ( frame ) % cols ) * ( src->w / cols ),
		(	( ( !matrix ) * ( int ( frame ) / cols ) + matrix * line ) *
			( src->h / rows )	),
		( src->w / cols ),
		( src->h / rows )
	);
}

int Animation::frameAmount () const
{
	return( ( matrix * cols ) + ( ( !matrix ) * ( rows * cols ) ) );
}

int Animation::getFrame () const
{
	return int ( frame );
}

int Animation::getFrameSize () const
{
	return int ( 1000.0f / fps );
}

void Animation::setFrame (int frame)
{
	if( ( !matrix ) || ( frame < cols ) )
	{
		this->frame = float ( frame );
		update_ ();
	}
}

void Animation::setFrameSize (int framesize)
{
	fps = 1000.0f / float ( framesize );
}
