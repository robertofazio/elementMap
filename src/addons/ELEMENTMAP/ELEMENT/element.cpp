#include "element.h"


void element::init(int _type,int _width, int _height, int _internalFormat, string _name,bool _isStereo, bool _isWarpable)
{
	printf("\n································································\n");
	printf(">> element init :: %s :: w:%d - h:%d \n",_name.c_str(),_width,_height);

	elementWidth	= _width;
	elementHeight	= _height;
	internalFormat	= _internalFormat;
	type			= _type;
	elementName		= _name;
	opacity			= 1.0;
    r               = 255;
    g               = 255;    
    b               = 255;
	blendingMode	= OF_BLENDMODE_DISABLED;
	swapLeftRight	= false;
	isShow			= true;
	isActive		= true;
	isClear			= false;
	isStereo		= _isStereo;
	drawInStereo	= _isStereo;
	blendingMode	= OF_BLENDMODE_DISABLED;
    isWarpable      = _isWarpable;
    isSelected      = false;
    
    if (isWarpable) 
        {
            warper.setup(elementWidth, elementHeight, _name);
        }
    
		fboLeft.allocate(elementWidth,elementHeight, internalFormat);
		fboRight.allocate(elementWidth,elementHeight, internalFormat);
		// clear the buffers from past (thnkx to matteo)
		fboLeft.begin();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fboLeft.end();
		
		fboRight.begin();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fboRight.end();

		
	setupUI(this);
	
	printf("································································\n");
    
    this->initFont();
}

void element::resetOutput(int newWidth, int newHeight)
{
    if (isWarpable) warper.resetOutput(newWidth, newHeight);
}

void element::initFont()
{
    georgiaitalic8.loadFont("georgiaz.ttf", 7);
    georgiaitalic10.loadFont("georgiaz.ttf", 9);
    
	georgiaitalic8.setLineHeight(12.0f);
	georgiaitalic8.setLetterSpacing(1.017);
    
	georgiaitalic10.setLineHeight(12.0f);
	georgiaitalic10.setLetterSpacing(1.017);   
}

void element::drawGraphic(int x, int y, int w, int h)
{
    ofSetColor(255, 255, 255);
    switch(this->type)
    {
        case 0:
            break;
        case 1:
            georgiaitalic10.drawString("element.video", x , y + 11);
            break;
        case 2:
            georgiaitalic10.drawString("element.image", x , y + 11);
            break;
        case 3:
            georgiaitalic10.drawString("element.syphon", x , y + 11);
            break;
        case 5:
            georgiaitalic10.drawString("element.Map Main Preview", x , y);
            break;
    }
    
    
    if(type != 5)   //disegna la preview
    {
        ofPushMatrix();
        ofTranslate(0, 20);
        ofTexture   leftTemp;
        leftTemp=this->getLeftTexture();
        leftTemp.draw(x, y, w, h);
        ofPopMatrix();
    }
}

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
void element::setRed(int red)
{
	r = red;	
}
//-----------------------------------------------------------------------
void element::setGreen(int green)
{
	g = green;	
}
//-----------------------------------------------------------------------
void element::setBlue(int blue)
{
	b = blue;	
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
void element::setBlendingMode(ofBlendMode _bm)
{
	blendingMode = _bm;
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
int element::getRed()
{
	return (r);
}
//-----------------------------------------------------------------------
int element::getGreen()
{
	return (g);
}
//-----------------------------------------------------------------------
int element::getBlue()
{
	return (b);
}
//-----------------------------------------------------------------------
string element::getElementName()
{
	return (elementName);	
}
//-----------------------------------------------------------------------
ofBlendMode element::getBlendingMode()
{
	return (blendingMode);	
}
