#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>

#include "Sprite.hpp"

class Animation : public Sprite
{
private:
	float frame;
	float fps;
	int rows;
	int cols;
	bool matrix;
public:
	int line;
	
	Animation (
		const std::string& filename,
		int frame,
		int framesize,
		int rows,
		int cols,
		bool matrix = false,
		int line = 0
	);
	
	void update ();
private:
	void update_ ();
	
	int frameAmount () const;
public:
	int getFrame () const;
	int getFrameSize () const;
	
	void setFrame (int frame);
	void setFrameSize (int framesize);
};

#endif
