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
//-----------------------------------------------------------------
void elementImage::drawIntoFbo(bool _drawMonoOrStereo)
{

	if(isActive)
	{
		// update the clear state of the object 
		if(isClear) isClear=false;
		
		this->setDrawInStereo(_drawMonoOrStereo);

		if(getDrawInStereo())
		{
			
			if(!getSwapLeftRight())
			{
				// stereo
				fboLeft.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				leftImage.draw(0,0,getWidth(),getHeight());
				fboLeft.end();
				
				//----------
				
				fboRight.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				rightImage.draw(0,0,getWidth(),getHeight());
				fboRight.end();
				
			}
			else
			{
				// stereo swapped
				fboLeft.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				rightImage.draw(0,0,getWidth(),getHeight());
				fboLeft.end();
				
				//----------
				
				fboRight.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				leftImage.draw(0,0,getWidth(),getHeight());
				fboRight.end();
				
			}
		}
		else 
		{
			// mono
			fboLeft.begin();
			ofBackground(0,0,0,0);
			setOpacityColor();
			leftImage.draw(0,0,getWidth(),getHeight());
			fboLeft.end();
		}
	}
	else if(!isClear)
	{
		fboLeft.begin();
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		fboLeft.end();
		
		fboRight.begin();
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		fboRight.end();
		
		isClear=true;
	}

}
*/


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

//-----------------------------------------------------------------
void elementImage::drawStereo(int x, int y, int w, int h)
{
    fboLeftAnagliph.begin();
    
    //  ofClear(0, 0, 0);
    // left
    glColorMask(true, false, false, false);
    //ofSetupGraphicDefaults();    
    //    setOpacityColor();
    leftImage.draw(0, 0);
    
    
    // right
    glColorMask(false, true, true, false);
    //ofSetupGraphicDefaults();
    //  setOpacityColor();
    rightImage.draw(0, 0);
    glColorMask(true, true, true, true);
    
    fboLeftAnagliph.end();
    fboLeftAnagliph.draw(x, y, w, h);
}
