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
	
    else {
    fboLeftAnagliph.allocate(elementWidth,elementHeight, internalFormat);
    fboLeftAnagliph.begin();
    glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
    fboLeftAnagliph.end();
    }
		
	setupUI(this);
	
	printf("································································\n");
    
    this->initFont();
}

void element::initFont()
{
    myFont.loadFont("verdana.ttf", 14);
	myFont.setLineHeight(12.0f);
	myFont.setLetterSpacing(1.037);
}

void element::drawGraphic(int x, int y, int w, int h)
{
    ofSetColor(255, 255, 255);
    myFont.loadFont("verdana.ttf", 10);
    switch(this->type)
    {
        case 0:
            break;
        case 1:
            myFont.drawString("element.video", x , y + 11);
            myFont.drawString("element.video Preview", x + 448, y + 11);
            break;
        case 2:
            myFont.drawString("element.image", x , y + 11);
            myFont.drawString("element.image Preview", x + 448, y + 11);
            break;
        case 3:
            myFont.drawString("element.syphon", x , y + 11);
            myFont.drawString("element.syphon Preview", x + 448, y + 11);
            break;
        case 5:
            myFont.drawString("element.Map Main Preview", x , y);
            break;
    }
    
    
    if(type != 5)
    {
        ofPushMatrix();
        ofTranslate(0, 20);
        this->drawLeft(x, y, w, h);
        if(this->isStereo)
            this->drawRight(x + w + 4, y, w, h);
        
        
        myFont.loadFont("verdana.ttf", 8);
        myFont.drawString("Left", x , y + h+ 14);
        if(this->isStereo)
            myFont.drawString("Right", x + w + 4 , y + h+ 14);
        
        ofPushMatrix();
//        ofTranslate(450, 0);
//        this->drawLeft(x, y, w, h);
        
        ofTranslate(450, 0);
        if(!this->getDrawInStereo())
            this->drawLeft(x, y, w, h);
            else 
            this->drawStereo(x, y, w, h);
        ofPopMatrix();
        
        ofPopMatrix();
        myFont.loadFont("verdana.ttf", 14);
    }
    else 
    {
        ofPushMatrix();
        ofTranslate(x, y + 10);
        this->drawPreview(0, 0, w, h);
        

        ofPopMatrix();
    }
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



void element::drawStereo(int x, int y, int w, int h) {}


