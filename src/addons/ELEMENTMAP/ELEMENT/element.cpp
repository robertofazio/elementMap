#include "element.h"


void element::init(int _type,int _width, int _height, int _internalFormat)
{
	elementWidth	= _width;
	elementHeight	= _height;
	internalFormat	= _internalFormat;
	type			= _type;
	swapLeftRight	= false;
	
	fboLeft.allocate(elementWidth,elementHeight, internalFormat);
	fboRight.allocate(elementWidth,elementHeight, internalFormat);
	
	printf("element w:%d - h:%d \n",elementWidth,elementHeight);
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
bool element::getSwapLeftRight()
{
	return swapLeftRight;
}