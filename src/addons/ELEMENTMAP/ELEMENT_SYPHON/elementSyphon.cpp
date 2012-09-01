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

	syphonClient.setup();
	syphonClient.setApplicationName(_applicationName);
	syphonClient.setServerName(_serverName);
	syphonClient.getTextureReference().allocate(_width,_height,GL_RGBA);
	
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
    if (isWarpable) warper.draw(getLeftTexture());
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
	return (syphonClient.getTextureReference());
}

//-----------------------------------------------------------------
ofTexture& elementSyphon::getRightTexture()
{
	syphonClient.bind();
	return (syphonClient.getTextureReference());	
}



