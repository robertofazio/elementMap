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
		//rightChannelPlayer.loadMovie(_rightChannel,OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
		rightChannelPlayer.loadMovie(_rightChannel);
		rightChannelPlayer.play();
	}
	
	//leftChannelPlayer.loadMovie(_leftChannel,OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY);
	leftChannelPlayer.loadMovie(_leftChannel);
	leftChannelPlayer.play();
	
	while(!leftChannelPlayer.isLoaded())
	{
		printf("·");
	}
	
	// UI params
	xPos = _xPos;
	yPos = _yPos;
	
//	this->init(1,int(leftChannelPlayer.getWidth()),int(leftChannelPlayer.getHeight()),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,this->getIsStereo());
	this->init(1,int(_width),int(_height),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,this->getIsStereo(), _isWarpable);
    
    isPaused=false;
    

	
}

void elementVideo::addFX(int type)
{
    
}

void elementVideo::applyFX()
{
    
}

/*
void elementVideo::drawPreview(int x, int y, int w, int h)
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
        ofPushMatrix();
        ofSetColor(0, 0, 0,0);
        ofRect(0,0,w,h);
        ofPopMatrix();
        
        warper.draw(getLeftTexture());
        fboLeft.end();
        
        fboLeft.draw(x,y,w,h);
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
        ofPushMatrix();
        ofSetColor(0, 0, 0,0);
        ofRect(0,0,w,h);
        ofPopMatrix();
        
        warper.draw(getRightTexture());
        fboRight.end();
        
        fboRight.draw(x,y,w,h);
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