#include "element.h"

//#include "ofConstants.h"


void element::init(int _type,int _width, int _height, int _internalFormat, string _name,int _inputType, bool _isWarpable)
{
	printf("\n································································\n");
	printf(">> element init :: %s :: w:%d - h:%d \n",_name.c_str(),_width,_height);

	elementWidth	= _width;
	elementHeight	= _height;
	internalFormat	= _internalFormat;
	type			= _type;
    inputType       = _inputType;
	elementName		= _name;
	opacity			= 1.0;
    lastOpacity     = opacity;
    r               = 255;
    g               = 255;    
    b               = 255;
	blendingMode	= OF_BLENDMODE_DISABLED;
	swapLeftRight	= false;
	isShow			= true;
	isActive		= true;
	isClear			= false;
    somethingHasChanged= false;
        
      if(inputType==0) isStereo=false;
      else isStereo=true;


    drawInStereo	= isStereo;
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
    
    noMedia.loadImage("./utils/noMedia.png");
	
	printf("································································\n");
    
    this->initFont();
}


void element::initFont()
{
//    fontMedium.loadFont("georgiaz.ttf", 9);        
//    fontMedium.loadFont("CODEBold.otf", 10);        
    fontMedium.loadFont("NEOSANS.otf", 10);        
	fontMedium.setLineHeight(12.0f);
	fontMedium.setLetterSpacing(1.017);   

}

void element::drawGraphic(int x, int y, int w, int h)
{
    
    if(this->somethingHasChanged)
    {
        this->somethingHasChanged = false;
        this->warper.createGrid(this->warper.xRes, this->warper.yRes);
    }
    
    ofPushStyle();
    ofSetColor(255,255,255,255);
    switch(this->type)
    {
        case 0:
            break;
        case 1:
            fontMedium.drawString("element.VIDEO", x , y+5);
            break;
        case 2:
            fontMedium.drawString("element.IMAGE", x , y+5);
            break;
        case 3:
            fontMedium.drawString("element.SYPHON", x , y+5);
            break;
        case 5:
            fontMedium.drawString("element.Map MAIN PREVIEW", x , y);
            break;
    }
    ofPopStyle();
    
    if(type != 5)   //disegna la preview
    {
        ofPushMatrix();
        
        ofTranslate(0, 40);
        ofPushStyle();
        ofSetColor(0, 0, 0, 255);
        ofRect(x, y, w, h);
        ofPopStyle();
        getLeftTexture().draw(x, y, w, h);
        
        if (type!= 3) 
        {
            ofTranslate(105, 0);
            ofPushStyle();
            ofSetColor(0, 0, 0, 255);
            ofRect(x, y, w, h);
            ofPopStyle();
//            if (parentElement->inputType == ELM_INPUT_MONO) noMedia.draw(x, y, w, h);
//            else getRightTexture().draw(x, y, w, h);
            getRightTexture().draw(x, y, w, h);

        
        }
        
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
//void element::setElementInputType(int _inType)
//{
//	inputType = _inType; 
//}
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
void element::memOpacity(float f)
{
	lastOpacity = f;	
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
int element::getElementInputType()
{
	return (inputType);
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
float element::getLastOpacity()
{
	return (lastOpacity);
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



//SAVE
//-----------------------------------------------------------------------
void element::saveSettings()
{
    XML.addTag("SETTINGS");
    XML.pushTag("SETTINGS");
    
    XML.addValue("drawInStereo", drawInStereo);
    
    XML.addValue("r", r);
    XML.addValue("g", g);
    XML.addValue("b", b);
    
    XML.addValue("opacity", opacity);
    
    XML.addValue("blendingMode", blendingMode);
    
    XML.addValue("isActive", isActive);
    
    XML.popTag();
    
    string XMLpath = "./XML/"+elementName+"_settings.xml";
    XML.saveFile(XMLpath);
}

//-----------------------------------------------------------------------
void element::loadSettings()
{
    ofxXmlSettings tempXML;
    string XMLpath = "./XML/"+elementName+"_settings.xml";
    tempXML.loadFile(XMLpath);
    
    tempXML.pushTag("SETTINGS");
    drawInStereo = tempXML.getValue("dawInStereo", true);
    
    r = tempXML.getValue("r", 255);
    g = tempXML.getValue("g", 255);
    b = tempXML.getValue("b", 255);
    
    
    opacity = tempXML.getValue("opacity", 1);
    
    int blendingModeInt = tempXML.getValue("blendingMode", 0);
    switch (blendingModeInt) {
        case 0:
            blendingMode=OF_BLENDMODE_DISABLED;
            break;
        case 1:
            blendingMode=OF_BLENDMODE_ALPHA;
            break;
        case 2:
            blendingMode=OF_BLENDMODE_ADD;
            break;
        case 3:
            blendingMode=OF_BLENDMODE_SUBTRACT;
            break;
        case 4:
            blendingMode=OF_BLENDMODE_MULTIPLY;
            break;
        case 5:
            blendingMode=OF_BLENDMODE_SCREEN;
            break;
        default:
            break;
    }
    
    isActive = tempXML.getValue("isActive", true);
    
    tempXML.popTag();
}

