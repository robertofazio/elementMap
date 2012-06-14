#ifndef ELEMENTIMAGE
#define ELEMENTIMAGE


#include "ofMain.h"
#include "element.h"


class elementImage : public element
{
	public :
	
	elementImage();
	
	void setup(string _leftImage, string _rightImage, bool _isStereo);
	void drawIntoFbo(bool _drawMonoOrStereo);
	void update();
	
	ofImage leftImage;
	ofImage rightImage;

};

#endif;