#include "elementImage.h"



//-----------------------------------------------------------------
elementImage::elementImage()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
//void elementImage::setup(string _leftImage, string _rightImage, int _width, int _height, bool _isStereo,int _posX, int _posY,string _name, bool _isWarpable)
void elementImage::setup(string _leftImage, string _rightImage, int _width, int _height, int inputType, int _posX, int _posY,string _name, bool _isWarpable)
{
	// UI params
	xPos = _posX;
	yPos = _posY;

    this->init(2,int(_width),int(_height),GL_RGBA,_name,inputType, _isWarpable);	

	if(getIsStereo())
	{
		rightImage.loadImage(_rightImage);
	}
	leftImage.loadImage(_leftImage);

}

//-----------------------------------------------------------------
void elementImage::update()
{
    if (isWarpable) warper.updateCoordinates();    
}


//-----------------------------------------------------------------
void elementImage::drawLeft(int x, int y, int w, int h)
{
        fboLeft.begin();
        if (isWarpable) warper.draw(getLeftTexture());
        else leftImage.draw(x,y,w,h);	
        fboLeft.end();

        ofPushStyle();
        ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
        fboLeft.draw(x,y,w,h);
        ofPopStyle();        
}


//-----------------------------------------------------------------
void elementImage::drawRight(int x, int y, int w, int h)
{
        fboRight.begin();
        if (isWarpable) warper.draw(getRightTexture());
        else rightImage.draw(x,y,w,h);	
        fboRight.end();
        
        ofPushStyle();
        ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
        fboRight.draw(x,y,w,h);
        ofPopStyle();
}

//-----------------------------------------------------------------
ofTexture& elementImage::getLeftTexture()
{
	return (leftImage.getTextureReference());	
}

//-----------------------------------------------------------------
ofTexture& elementImage::getRightTexture()
{
	return (rightImage.getTextureReference());	
}
