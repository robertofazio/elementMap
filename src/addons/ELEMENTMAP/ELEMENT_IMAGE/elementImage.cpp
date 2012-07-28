#include "elementImage.h"



//-----------------------------------------------------------------
elementImage::elementImage()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementImage::setup(string _leftImage, string _rightImage, int _width, int _height, bool _isStereo,int _posX, int _posY,string _name, bool _isWarpable)
{
	setIsStereo(_isStereo);
	setDrawInStereo(_isStereo);
	
	if(getIsStereo())
	{
		rightImage.loadImage(_rightImage);
	}
	leftImage.loadImage(_leftImage);
	
	// UI params
	xPos = _posX;
	yPos = _posY;
    
//	this->init(2,int(leftImage.getWidth()),int(leftImage.getHeight()),GL_RGBA,_name,this->getIsStereo());	
    this->init(2,int(_width),int(_height),GL_RGBA,_name,this->getIsStereo(), _isWarpable);	
}

//-----------------------------------------------------------------
void elementImage::update()
{
    if (isWarpable) warper.updateCoordinates();    
}

void elementImage::applyFX()
{
    for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
            effects[a]->applyFX();
    }
}

void elementImage::addFX(int type)       // Mauro
{
    switch(type)
    {
        case ELEMENT_FX_MASK:            
            newEffect.init(ELEMENT_FX_MASK, leftImage.getTextureReference());
            effects.push_back(&newEffect);
            break;
    }    
}
/*
 //-----------------------------------------------------------------
 void elementImage::drawGraphic(int x, int y, int w, int h)
 {
 //fboLeft.draw(x,y,w,h);
 this->drawLeft(x, y, w, h);
 }*/

//-----------------------------------------------------------------
void elementImage::drawLeft(int x, int y, int w, int h)
{
    
    if (isWarpable)
    {
        fboLeft.begin();
        ofPushMatrix();
        ofSetColor(0, 0, 0,0);
        ofRect(0,0,w,h);
        ofPopMatrix();
        
        warper.draw(getLeftTexture());
        fboLeft.end();
        
        fboLeft.draw(x,y,w,h);
    }
    else
        leftImage.draw(x,y,w,h);	
    
    this->applyFX();
}


//-----------------------------------------------------------------
void elementImage::drawRight(int x, int y, int w, int h)
{
    if (isWarpable)
    {
        
        fboRight.begin();
        ofPushMatrix();
        ofSetColor(0, 0, 0,0);
        ofRect(0,0,w,h);
        ofPopMatrix();
        
        warper.draw(getRightTexture());
        fboRight.end();
        
        fboRight.draw(x,y,w,h);
    }
    else
        rightImage.draw(x,y,w,h);	
    this->applyFX();
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
