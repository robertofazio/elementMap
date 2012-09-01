#include "elementVideo.h"


//-----------------------------------------------------------------
elementVideo::elementVideo()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementVideo::setup(string _leftChannel, string _rightChannel, int _width, int _height, int inputType,int _xPos, int _yPos,string _name, bool _isWarpable)
{
    
	// UI params
	xPos = _xPos;
	yPos = _yPos;
    
	this->init(1,int(_width),int(_height),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,inputType, _isWarpable);
	
	if(getIsStereo()) rightChannelPlayer.loadMovie(_rightChannel);
    leftChannelPlayer.loadMovie(_leftChannel);
    
    mute    =    false;
    volume  =    100;
    
    
}

//-----------------------------------------------------------------
void elementVideo::update()
{
    
    if (isWarpable) warper.updateCoordinates();

	leftChannelPlayer.idleMovie();
	rightChannelPlayer.idleMovie();
    
    if (mute) leftChannelPlayer.setVolume(0);
    else leftChannelPlayer.setVolume(volume);
    
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
    leftChannelPlayer.setSpeed(_speed);
    rightChannelPlayer.setSpeed(_speed);
    leftChannelPlayer.play();
    rightChannelPlayer.play();
}

//-----------------------------------------------------------------
void elementVideo::element_videoPause()
{
    leftChannelPlayer.stop();
    rightChannelPlayer.stop();   
}

//-----------------------------------------------------------------
void elementVideo::element_videoStop()
{
    leftChannelPlayer.play();
    rightChannelPlayer.play();
    leftChannelPlayer.setFrame(0);
    rightChannelPlayer.setFrame(0);
    leftChannelPlayer.stop();
    rightChannelPlayer.stop();
}

//-----------------------------------------------------------------
void elementVideo::element_frameAvanti()
{
    leftChannelPlayer.play();
    rightChannelPlayer.play();
    leftChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame()+1);
    rightChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame());
    leftChannelPlayer.stop();
    rightChannelPlayer.stop();    
}

//-----------------------------------------------------------------
void elementVideo::element_frameIndietro()
{
    leftChannelPlayer.play();
    rightChannelPlayer.play();
    leftChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame()-1);
    rightChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame());
    leftChannelPlayer.stop();
    rightChannelPlayer.stop();    
    
}

//-----------------------------------------------------------------
void elementVideo::element_goToFrame(int _frame)
{
    leftChannelPlayer.play();
    rightChannelPlayer.play();
    leftChannelPlayer.setFrame(_frame);
    rightChannelPlayer.setFrame(_frame);
    leftChannelPlayer.stop();
    rightChannelPlayer.stop();    
    
}

//-----------------------------------------------------------------
void elementVideo::element_setLoop(ofLoopType _loop)
{
    leftChannelPlayer.setLoopState(_loop);
    rightChannelPlayer.setLoopState(_loop);
}

//-----------------------------------------------------------------
void elementVideo::element_toggleSound()
{
    mute=!mute;
}

//-----------------------------------------------------------------
void elementVideo::element_soundVolume(float _volume)
{
    volume=_volume;    
}

