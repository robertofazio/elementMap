#include "elementImage.h"



//-----------------------------------------------------------------
elementImage::elementImage()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementImage::setup(string _leftImage, string _rightImage, int _width, int _height, int inputType, int _posX, int _posY,string _name, bool _isWarpable)
{

    //source files:
    leftImagePath= _leftImage;
    rightImagePath= _rightImage;
	
    // UI params
	xPos = _posX;
	yPos = _posY;

    this->init(2,int(_width),int(_height),GL_RGBA,_name,inputType, _isWarpable);	

	if(getElementInputType()==ELM_INPUT_STEREO_TWO_CHANNEL)
	{
		rightImage.loadImage(rightImagePath);
	}
        leftImage.loadImage(leftImagePath);

    if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        leftImage.resize(leftImage.getWidth()*2, leftImage.getHeight());
        rightImage.cropFrom(leftImage, leftImage.getWidth(), 0, leftImage.getWidth(), leftImage.getHeight());
        leftImage.crop(0, 0, leftImage.getWidth(), leftImage.getHeight());
    }
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        leftImage.resize(leftImage.getWidth(), leftImage.getHeight()*2);
        rightImage.cropFrom(leftImage, 0, leftImage.getHeight(), leftImage.getWidth(), leftImage.getHeight());
        leftImage.crop(0, 0, leftImage.getWidth(), leftImage.getHeight());
    }


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



//-----------------------------------------------------------------
void elementImage::setElementInputType(int _inType)
{
    inputType=_inType;
        
    if (getElementInputType() == ELM_INPUT_MONO)
    {
        leftImage.clear();
        leftImage.loadImage(leftImagePath);
        leftImage.resize(leftImage.getWidth(), leftImage.getHeight());
    }
    else if (getElementInputType() == ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        leftImage.clear();
        rightImage.clear();
        leftImage.loadImage(leftImagePath);
        leftImage.resize(leftImage.getWidth(), leftImage.getHeight());
        rightImage.loadImage(rightImagePath);        
    }
    else if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        leftImage.clear();
        rightImage.clear();
        leftImage.loadImage(leftImagePath);
        leftImage.resize(leftImage.getWidth()*2, leftImage.getHeight());
        rightImage.cropFrom(leftImage, leftImage.getWidth()*.5, 0, leftImage.getWidth()*.5, leftImage.getHeight());
        leftImage.crop(0, 0, leftImage.getWidth()*.5, leftImage.getHeight());
    }
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        leftImage.clear();
        rightImage.clear();
        leftImage.loadImage(leftImagePath);
        leftImage.resize(leftImage.getWidth(), leftImage.getHeight()*2);
        rightImage.cropFrom(leftImage, 0, leftImage.getHeight()*.5, leftImage.getWidth(), leftImage.getHeight()*.5);
        leftImage.crop(0, 0, leftImage.getWidth(), leftImage.getHeight()*.5);
    }

}
