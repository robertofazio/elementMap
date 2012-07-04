#include "elementImage.h"



//-----------------------------------------------------------------
elementImage::elementImage()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementImage::setup(string _leftImage, string _rightImage, bool _isStereo,int _posX, int _posY,string _name)
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
    
	this->init(2,int(leftImage.getWidth()),int(leftImage.getHeight()),GL_RGBA,_name,this->getIsStereo());	
}

//-----------------------------------------------------------------
void elementImage::update()
{
    
}
/*
 void elementImage::drawPreview(int x, int y, int w, int h)
 {
 for(int a = 0; a < effects.size(); a++)
 {
 if(effects[a]->getIsActive())
 {
 applyFX();
 effects[a]->finalFbo.draw(x, y, w, h);
 }
 else
 effects[a]->getLeftTexture().draw(x, y, w, h);
 if(effects[a]->getGUIVisible())
 {
 effects[a]->drawGUI(x, y, w, h);
 }
 }
 if(effects.size() == 0)
 {
 this->drawLeft(x, y, 100, 100 / (4/3));
 }
 }
 */

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
    //fboLeft.draw(x,y,w,h);
	leftImage.draw(x,y,w,h);	
    this->applyFX();
}


//-----------------------------------------------------------------
void elementImage::drawRight(int x, int y, int w, int h)
{
	//fboRight.draw(x,y,w,h);
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
