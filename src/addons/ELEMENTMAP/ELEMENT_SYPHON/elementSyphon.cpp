#include "elementSyphon.h"

// we are considering spyhon is not stereo !! it could be in a top/bottom or anaglyph state ?

//-----------------------------------------------------------------
elementSyphon::elementSyphon()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementSyphon::setup(string _applicationName,string _serverName,int _width,int _height,int _posX, int _posY,string _name, bool _isWarpable)
{
	setIsStereo(false);
	setDrawInStereo(false);

    syphonWidth=_width;
    syphonHeight=_height;
    
	syphonClient.setup();
	syphonClient.setApplicationName(_applicationName);
	syphonClient.setServerName(_serverName);
    syphonClient.getTextureReference().allocate(_width,_height,GL_RGBA);   

    if(getElementInputType()==ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        ofSystemAlertDialog("NOT SUPPORTED: syphhon has only one channel!");        
    }
    else if (getElementInputType()==ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempLeft.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
        lT.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
        rT.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
    }
    else if (getElementInputType()==ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempLeft.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
        lT.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
        rT.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
    }
	
	
	// UI params
	xPos = _posX;
	yPos = _posY;

	this->init(3,_width,_height,GL_RGBA,_name,0, _isWarpable);	

}



//-----------------------------------------------------------------
void elementSyphon::update()
{
    if (isWarpable)  warper.updateCoordinates();
}
//-----------------------------------------------------------------
void elementSyphon::drawLeft(int x, int y, int w, int h)
{

    fboLeft.begin();
    if (isWarpable) warper.draw(getLeftTexture());
    else syphonClient.draw(x,y,w,h);	
    fboLeft.end();
    
    ofPushStyle();
    ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
    fboLeft.draw(x,y,w,h);
    ofPopStyle();        

}



//-----------------------------------------------------------------
void elementSyphon::drawRight(int x, int y, int w, int h)
{
    fboRight.begin();
    if (isWarpable) warper.draw(getRightTexture());
    else syphonClient.draw(x,y,w,h);	
    fboRight.end();
    
    ofPushStyle();
    ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
    fboRight.draw(x,y,w,h);
    ofPopStyle();        
}



//-----------------------------------------------------------------
ofTexture& elementSyphon::getLeftTexture()
{
	syphonClient.bind();
    
    //se ho un ingresso mono, passo sempilcemente la texture in arrivo da syphon:
    if (getElementInputType() == ELM_INPUT_MONO || getElementInputType() == ELM_INPUT_STEREO_TWO_CHANNEL)
    {
	return (syphonClient.getTextureReference());
    }
    //invece se esporto in leftright estraggo la parte sinistra:
    else if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempLeft.begin();
        ofClear(0,0,0,0);
        syphonClient.getTextureReference().draw(0,0,syphonWidth,syphonHeight);
        tempLeft.end();
        
        lT.clear();
        lT=tempLeft.getTextureReference();
        
        return (lT);
    }
    //infine, se esporto in leftright estraggo la parte superiore:
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempLeft.begin();
        ofClear(0,0,0,0);
        syphonClient.getTextureReference().draw(0,0,syphonWidth,syphonHeight);
        tempLeft.end();
        
        lT.clear();
        lT=tempLeft.getTextureReference();

        return (lT);

    }


}

//-----------------------------------------------------------------
ofTexture& elementSyphon::getRightTexture()
{
	syphonClient.bind();
    
    //se ho un ingresso mono, passo sempilcemente la texture in arrivo da syphon:
    if (getElementInputType() == ELM_INPUT_MONO)
    {
        return (syphonClient.getTextureReference());
    }
    //invece se esporto in leftright estraggo la parte destra:
    else if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempRight.begin();
        ofClear(0,0,0,0);
        syphonClient.getTextureReference().draw(-syphonWidth*.5,0,syphonWidth,syphonHeight);
        tempRight.end();
        
        rT.clear();
        rT=tempRight.getTextureReference();
        
        return (rT);
    }
    //infine, se esporto in leftright estraggo la parte inferiore:
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempRight.begin();
        ofClear(0,0,0,0);
        syphonClient.getTextureReference().draw(0,-syphonHeight*.5,syphonWidth,syphonHeight);
        tempRight.end();
        
        rT.clear();
        rT=tempRight.getTextureReference();
        
        return (rT);
    }

}


//-----------------------------------------------------------------
void elementSyphon::setElementInputType(int _inType)
{
    inputType=_inType;
    
    if(getElementInputType()==ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        ofSystemAlertDialog("NOT SUPPORTED: syphhon has only one channel!");        
    }
    else if (getElementInputType()==ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempLeft.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
        tempRight.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
        lT.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
        rT.allocate(syphonWidth*.5, syphonHeight, GL_RGBA);
    }
    else if (getElementInputType()==ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempLeft.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
        tempRight.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
        lT.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
        rT.allocate(syphonWidth, syphonHeight*.5, GL_RGBA);
    }
    
}




