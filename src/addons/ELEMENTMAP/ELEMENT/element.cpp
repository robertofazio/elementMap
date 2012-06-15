#include "element.h"


void element::init(int _type,int _width, int _height, int _internalFormat)
{

	elementWidth	= _width;
	elementHeight	= _height;
	internalFormat	= _internalFormat;
	type			= _type;
	swapLeftRight	= false;
	opacity			= 1.0;
	hide			= false;
	
	fboLeft.allocate(elementWidth,elementHeight, internalFormat);
	fboRight.allocate(elementWidth,elementHeight, internalFormat);
	
	// clear the buffers from past 
	fboLeft.begin();
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
	fboLeft.end();

	fboRight.begin();
	glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
	fboRight.end();
	
	printf("element w:%d - h:%d \n",elementWidth,elementHeight);
}


//-----------------------------------------------------------------
void element::setOpacityColor()
{
	// put color based on opacity
	glColor4f(getOpacity(),getOpacity(),getOpacity(),getOpacity());
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
void element::setHide(bool b)
{
	hide = b;	
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
bool element::getHide()
{
	return hide;
}
//-----------------------------------------------------------------------
float element::getOpacity()
{
	return (opacity);
}
