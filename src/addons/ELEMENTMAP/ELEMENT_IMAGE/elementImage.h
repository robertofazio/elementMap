#ifndef ELEMENTIMAGE
#define ELEMENTIMAGE


#include "ofMain.h"
#include "element.h"


class elementImage : public element
{
	public :
	
	elementImage();
	
    void setup(string _leftImage, string _rightImage, int _width, int _height, int inputType, int _posX, int _posY,string _name, bool _isWarpable);
//	void setup(string _leftImage, string _rightImage, int _width, int _height, bool _isStereo,int _posX, int _posY,string _name, bool _isWarpable);
	void drawIntoFbo(bool _drawMonoOrStereo){};
	void update();
    //	void drawGraphic(int x, int y, int w, int h);
	void drawLeft(int x, int y, int w, int h);
	void drawRight(int x, int y, int w, int h);
	ofTexture& getLeftTexture();
	ofTexture& getRightTexture();
    
	ofImage leftImage;
	ofImage rightImage;
    
};

#endif