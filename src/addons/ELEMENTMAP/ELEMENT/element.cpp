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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fboLeft.end();
		
		fboRight.begin();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fboRight.end();
        
	}
	

		
	setupUI(this);
	
	printf("································································\n");
    
    this->initFont();
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
            georgiaitalic10.drawString("element.video Preview", x + 448, y + 11);
            break;
        case 2:
            georgiaitalic10.drawString("element.image", x , y + 11);
            georgiaitalic10.drawString("element.image Preview", x + 448, y + 11);
            break;
        case 3:
            georgiaitalic10.drawString("element.syphon", x , y + 11);
            georgiaitalic10.drawString("element.syphon Preview", x + 448, y + 11);
            break;
        case 5:
            georgiaitalic10.drawString("element.Map Main Preview", x , y);
            break;
    }
    
    
    if(type != 5)
    {
        ofPushMatrix();
        ofTranslate(0, 20);
        this->drawLeft(x, y, w, h);
        if(this->isStereo)
            this->drawRight(x + w + 4, y, w, h);
        
        georgiaitalic8.drawString("Left", x , y + h+ 14);
        
        if(this->isStereo)
            georgiaitalic8.drawString("Right", x + w + 4 , y + h+ 14);
        
        ofPushMatrix();
        ofTranslate(450, 0);
        this->drawLeft(x, y, w, h);
        ofPopMatrix();
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
