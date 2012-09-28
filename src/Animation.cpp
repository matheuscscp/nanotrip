#include <cmath>

#include "common.hpp"

#include "Animation.hpp"

#include "SDLBase.hpp"

using std::string;

Animation::Animation (
	const string& filename,
	int frame,
	int fps,
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
	this->fps = ((fps >= 1) ? fps : 1);
	
	Uint8 r,g,b,a;
	std::cout<<filename<<std::endl;
	for (int i=0; i < src->w; i++){
		for (int j=0; j < src->h; j++){
			SDL_GetRGBA(getPixel(i, j), src->format, &r, &g, &b, &a);
			if (1 || (a>0 && r<96 && abs(g-b)<48)){ //TODO color key
				tintPixels.push_back(std::pair<int,int>(i,j));
				tintColors.push_back(rgb2hsv({r/255.0,g/255.0,b/255.0,a/255.0}));
			}
		}
	}

	update_ ();
}

void Animation::update ()
{
	frame += fps*float(SDLBase::dt())/1000;
	if (frame >= cols)
		frame -= cols;
	update_();
}

void Animation::update_ ()
{
	clip (
		(int(frame)%cols)*(src->w/cols),
		((!matrix)*(int(frame)/cols)+(matrix*line))*(src->h/rows),
		src->w/cols,
		src->h/rows
	);
}

int Animation::frameAmount () const { return ((matrix*cols) + ((!matrix)*(rows*cols))); }
int Animation::getFrame() const { return frame; }
int Animation::getFPS() const { return fps; }
int Animation::getTimeSize() const { return (frameAmount()*1000)/fps; }

void Animation::setFrame(int frame)
{
	if( ( !matrix ) || ( frame < cols ) )
	{
		this->frame = float ( frame );
		update_();
	}
}

void Animation::setFPS(int fps) {
	this->fps = ((fps >= 1) ? fps : 1);
}


/**
 * 
 */
void Animation::tint(float hueShift){
	int hueDelta = 360*hueShift;

	hsv hsvColor;
	rgb rgbColor;
	if (fabs(lastHueShift-hueShift)>0.003){
		lastHueShift = hueShift;
		int size = tintPixels.size();
		for (int i=hueInterlace; i<size; i+=hueInterlaceFactor ){
			hsvColor = tintColors[i];
			hsvColor.h = fmod((hsvColor.h-hueDelta),360);
			rgbColor = hsv2rgb(hsvColor);
			setPixel(tintPixels[i].first, tintPixels[i].second, SDL_MapRGBA(src->format, rgbColor.r*255, rgbColor.g*255, rgbColor.b*255, hsvColor.a*255));
		}
		hueInterlace = ++hueInterlace%hueInterlaceFactor;
	}
}
