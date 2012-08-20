#include "elementVideo.h"


//-----------------------------------------------------------------
elementVideo::elementVideo()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementVideo::setup(string _leftChannel, string _rightChannel, int _width, int _height, bool _isStereo,int _xPos, int _yPos,string _name, bool _isWarpable)
{
	setIsStereo(_isStereo);
	setDrawInStereo(_isStereo);
	
	if(getIsStereo()) rightChannelPlayer.loadMovie(_rightChannel);
    leftChannelPlayer.loadMovie(_leftChannel);
	
	// UI params
	xPos = _xPos;
	yPos = _yPos;
	
	this->init(1,int(_width),int(_height),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,this->getIsStereo(), _isWarpable);
    
}

//-----------------------------------------------------------------
void elementVideo::update()
{
    
    if (isWarpable) warper.updateCoordinates();

	leftChannelPlayer.idleMovie();
	rightChannelPlayer.idleMovie();
}

//-----------------------------------------------------------------
void elementVideo::drawLeft(int x, int y, int w, int h)
{
    
    fboLeft.begin();
    if (isWarpable) warper.draw(getLeftTexture());
    else leftChannelPlayer.draw(x,y,w,h);	
    fboLeft.end();
    
    ofPushStyle();
    ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
    fboLeft.draw(x,y,w,h);
    ofPopStyle();        
}

//-----------------------------------------------------------------
void elementVideo::drawRight(int x, int y, int w, int h)
{
    fboRight.begin();
    if (isWarpable) warper.draw(getRightTexture());
    else rightChannelPlayer.draw(x,y,w,h);	
    fboRight.end();
    
    ofPushStyle();
    ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
    fboRight.draw(x,y,w,h);
    ofPopStyle();
}


//-----------------------------------------------------------------
ofTexture& elementVideo::getLeftTexture()
{
	return (leftChannelPlayer.getTextureReference());	
}

//-----------------------------------------------------------------
ofTexture& elementVideo::getRightTexture()
{
	return (rightChannelPlayer.getTextureReference());	
}

// PLAYER UTILITIES //

//-----------------------------------------------------------------
void elementVideo::element_videoPlay(int _speed)
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_videoPause()
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_videoStop()
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_frameAvanti()
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_frameIndietro()
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_setLoop(ofLoopType _loop)
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_toggleSound()
{
    
}

//-----------------------------------------------------------------
void elementVideo::element_soundVolume(float _volume)
{
    
}

