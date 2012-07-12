#ifndef ELEMENTIMAGE
#define ELEMENTIMAGE


#include "ofMain.h"
#include "element.h"


class elementImage : public element
{
	public :
	
	elementImage();
	
	void setup(string _leftImage, string _rightImage, bool _isStereo,int _posX, int _posY,string _name);
	void drawIntoFbo(bool _drawMonoOrStereo){};
	void update();
    //	void drawGraphic(int x, int y, int w, int h);
	void drawLeft(int x, int y, int w, int h);
	void drawRight(int x, int y, int w, int h);
    void addFX(int type);       // Mauro
    void    applyFX(); // Mauro
	ofTexture& getLeftTexture();
	ofTexture& getRightTexture();
    
	ofImage leftImage;
	ofImage rightImage;
    
};

#endif;