#include "elementVideo.h"


//-----------------------------------------------------------------
elementVideo::elementVideo()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementVideo::setup(string _leftChannel, string _rightChannel, bool _isStereo,int _xPos, int _yPos,string _name)
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
	

	this->init(1,int(leftChannelPlayer.getWidth()),int(leftChannelPlayer.getHeight()),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,this->getIsStereo());	
	
}

void elementVideo::addFX(int type)
{
    
}

void elementVideo::applyFX()
{
    
}

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

//-----------------------------------------------------------------
void elementVideo::update()
{
	leftChannelPlayer.idleMovie();
	rightChannelPlayer.idleMovie();
    
    fboLeftAnagliph.begin();
    
    //  ofClear(0, 0, 0);
    // left
    glColorMask(true, false, false, false);
    //ofSetupGraphicDefaults();    
    //  setOpacityColor();
    leftChannelPlayer.draw(0, 0);
    
    // right
    glColorMask(false, true, true, false);
    //ofSetupGraphicDefaults();
    //setOpacityColor();
    rightChannelPlayer.draw(0, 0);
    
    glColorMask(true, true, true, true);
    
    fboLeftAnagliph.end();

}


/*
//-----------------------------------------------------------------
void elementVideo::drawGraphic(int x, int y, int w, int h)
{
	this->drawLeft(x,y,w,h);
}
*/

//-----------------------------------------------------------------
void elementVideo::drawLeft(int x, int y, int w, int h)
{
	leftChannelPlayer.draw(x,y,w,h);
}

//-----------------------------------------------------------------
void elementVideo::drawRight(int x, int y, int w, int h)
{
	rightChannelPlayer.draw(x,y,w,h);		
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


//-----------------------------------------------------------------
void elementVideo::drawStereo(int x, int y, int w, int h)
{


fboLeftAnagliph.draw(x, y, w, h);
//leftChannelPlayer.draw(x,y,w,h);
}
