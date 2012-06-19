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
	
	xPos = _xPos;
	yPos = _yPos;
	
	//elementUIBase::setGUIOrigin(_xPos,_yPos);

	this->init(1,int(leftChannelPlayer.getWidth()),int(leftChannelPlayer.getHeight()),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name);	
	
}

//-----------------------------------------------------------------
void elementVideo::update()
{
	leftChannelPlayer.idleMovie();
	rightChannelPlayer.idleMovie();
}

//-----------------------------------------------------------------
void elementVideo::drawIntoFbo(bool _drawMonoOrStereo)
{
	if(isActive)
	{
		// update the clear state of the object 
		if(isClear) isClear=false;

		this->setDrawInStereo(_drawMonoOrStereo);

		if(this->getDrawInStereo())
		{
			// stereo
			if(!getSwapLeftRight())
			{
				fboLeft.begin();
				setOpacityColor();
				leftChannelPlayer.draw(0,0,getWidth(),getHeight());
				fboLeft.end();
				
				//----------
				
				fboRight.begin();
				setOpacityColor();
				rightChannelPlayer.draw(0,0,getWidth(),getHeight());
				fboRight.end();
			}
			else 
			{
				fboLeft.begin();
				setOpacityColor();
				rightChannelPlayer.draw(0,0,getWidth(),getHeight());
				fboLeft.end();
				
				//----------
				
				fboRight.begin();
				setOpacityColor();
				leftChannelPlayer.draw(0,0,getWidth(),getHeight());
				fboRight.end();
			}

		}
		else 
		{
			// mono
			fboLeft.begin();
			setOpacityColor();
			leftChannelPlayer.draw(0,0,getWidth(),getHeight());
			fboLeft.end();

		}
	}
	else if(!isClear)
	{
		fboLeft.begin();
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		fboLeft.end();
		
		fboRight.begin();
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		fboRight.end();
		
		isClear=true;
	}
	
	
}


//-----------------------------------------------------------------
void elementVideo::setPause(bool _b)
{
    leftChannelPlayer.setPaused(_b);
	rightChannelPlayer.setPaused(_b);
    
}

//-----------------------------------------------------------------
bool elementVideo::getIsPaused()
{
    return leftChannelPlayer.isPaused();
}


//-----------------------------------------------------------------
void elementVideo::rewind()
{
    leftChannelPlayer.setFrame(0);
    rightChannelPlayer.setFrame(0);
}