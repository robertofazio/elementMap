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
	
	if(getIsStereo())
	{
		rightChannelPlayer.loadMovie(_rightChannel);
		rightChannelPlayer.play();
	}
    leftChannelPlayer.loadMovie(_leftChannel);
	leftChannelPlayer.play();
	
	// UI params
	xPos = _xPos;
	yPos = _yPos;
	
	this->init(1,int(_width),int(_height),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,this->getIsStereo(), _isWarpable);
    
    isPaused=false;
}

//-----------------------------------------------------------------
void elementVideo::applyFX()
{
    for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
            effects[a]->applyFX();
    }
}

//-----------------------------------------------------------------
void elementVideo::addFX(int type)       // Mauro
{
    switch(type)
    {
        case ELEMENT_FX_MASK:            
            newEffect.init(ELEMENT_FX_MASK, leftChannelPlayer.getTextureReference());
            effects.push_back(&newEffect);
            break;
    }    
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
    
    if (isWarpable)
    {
        
        fboLeft.begin();
        warper.draw(getLeftTexture());
        fboLeft.end();
        
        ofPushStyle();
        int _opacity=int(ofMap(getOpacity(), 0, 1, 0, 255));
        ofSetColor(255, 255, 255,_opacity);
        fboLeft.draw(x,y,w,h);
        ofPopStyle();

    }
    else
        leftChannelPlayer.draw(x,y,w,h);
    
    this->applyFX();
}

//-----------------------------------------------------------------
void elementVideo::drawRight(int x, int y, int w, int h)
{
    if (isWarpable)
    {
        
        fboRight.begin();
        warper.draw(getRightTexture());
        fboRight.end();
        
        int _opacity=int(ofMap(getOpacity(), 0, 1, 0, 255));
        ofSetColor(255, 255, 255,_opacity);
        fboRight.draw(x,y,w,h);
        ofPopStyle();
    }
	else
        rightChannelPlayer.draw(x,y,w,h);		
    
    this->applyFX();
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