#include "element.h"


void element::init(int _type,int _width, int _height, int _internalFormat, string _name,bool _isStereo)
{
	printf("\n································································\n");
	printf(">> element init :: %s :: w:%d - h:%d \n",_name.c_str(),_width,_height);

	elementWidth	= _width;
	elementHeight	= _height;
	internalFormat	= _internalFormat;
	type			= _type;
	elementName		= _name;
	opacity			= 1.0;
	blendingMode	= 0;
	swapLeftRight	= false;
	isShow			= true;
	isActive		= true;
	isClear			= false;
	isStereo		= _isStereo;
	drawInStereo	= _isStereo;
	blendingMode	= 0;
    
	if(type==5)
	{
		
		fboLeft.allocate(elementWidth,elementHeight, internalFormat);
		fboRight.allocate(elementWidth,elementHeight, internalFormat);
		// clear the buffers from past (thnkx to matteo)
		fboLeft.begin();
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
		fboLeft.end();
		
		fboRight.begin();
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
		fboRight.end();
		
	}
	
		
	setupUI(this);
	
	printf("································································\n");
}


/*
void element::drawPreview(int x, int y, int w, int h)
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
}
*/

/*
void element::applyFX()
{
    for(int a = 0; a < effects.size(); a++)
    {
       if(effects[a]->getIsActive())
            effects[a]->applyFX();
    }
}
*/


//-----------------------------------------------------------------
void element::setOpacityColor()
{
	// put color based on opacity
	glColor4f(getOpacity(),getOpacity(),getOpacity(),1.0f);
}

//
// SETs 
//

//-----------------------------------------------------------------------
void element::setIsStereo(bool _b)
{
	isStereo = _b;
}
//-----------------------------------------------------------------------
void element::setDrawInStereo(bool _b)
{
	drawInStereo = _b;
}
//-----------------------------------------------------------------------
void element::setWidth(int _i)
{
	elementWidth = _i;
}
//-----------------------------------------------------------------------
void element::setHeight(int _i)
{
	elementHeight = _i;
}
//-----------------------------------------------------------------------
void element::setInternalFormat(int _i)
{
	internalFormat = _i;
}
//-----------------------------------------------------------------------
void element::setSwapLeftRight(bool b)
{
	swapLeftRight = b;	
}
//-----------------------------------------------------------------------
void element::setOpacity(float f)
{
	opacity = f;	
}
//-----------------------------------------------------------------------
void element::setIsShow(bool b)
{
	isShow = b;	
}
//-----------------------------------------------------------------------
void element::setIsActive(bool b)
{
	isActive = b;	
}
//-----------------------------------------------------------------------
void element::setBlendingMode(int _i)
{
	blendingMode = _i;
}



//
// GETs 
//

//-----------------------------------------------------------------------
bool element::getIsStereo()
{
	return (isStereo);
}
//-----------------------------------------------------------------------
bool element::getDrawInStereo()
{
	return (drawInStereo);
}
//-----------------------------------------------------------------------
int element::getWidth()
{
	return (elementWidth);
}
//-----------------------------------------------------------------------
int element::getHeight()
{
	return (elementHeight);
}
//-----------------------------------------------------------------------
int element::getInternalFormat()
{
	return (internalFormat);
}
//-----------------------------------------------------------------------
int element::getElementType()
{
	return (type);
}
//-----------------------------------------------------------------------
bool element::getSwapLeftRight()
{
	return swapLeftRight;
}//-----------------------------------------------------------------------
bool element::getIsShow()
{
	return isShow;
}
//-----------------------------------------------------------------------
bool element::getIsActive()
{
	return isActive;
}
//-----------------------------------------------------------------------
float element::getOpacity()
{
	return (opacity);
}
//-----------------------------------------------------------------------
string element::getElementName()
{
	return (elementName);	
}
//-----------------------------------------------------------------------
int element::getBlendingMode()
{
	return (blendingMode);	
}



