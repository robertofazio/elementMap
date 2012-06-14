#include "elementVideo.h"


//-----------------------------------------------------------------
elementVideo::elementVideo()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementVideo::setup(string _leftChannel, string _rightChannel, bool _isStereo)
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
	this->init(1,int(leftChannelPlayer.getWidth()),int(leftChannelPlayer.getHeight()),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal);	
	
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

	this->setDrawInStereo(_drawMonoOrStereo);
	
	if(this->getDrawInStereo())
	{
		// stereo
		if(!getSwapLeftRight())
		{
			fboLeft.begin();
			ofSetColor(255);
			leftChannelPlayer.draw(0,0,getWidth(),getHeight());
			fboLeft.end();
			
			//----------
			
			fboRight.begin();
			ofSetColor(255);
			rightChannelPlayer.draw(0,0,getWidth(),getHeight());
			fboRight.end();
		}
		else 
		{
			fboLeft.begin();
			ofSetColor(255);
			rightChannelPlayer.draw(0,0,getWidth(),getHeight());
			fboLeft.end();
			
			//----------
			
			fboRight.begin();
			ofSetColor(255);
			leftChannelPlayer.draw(0,0,getWidth(),getHeight());
			fboRight.end();
		}

	}
	else 
	{
		// mono
		fboLeft.begin();
		leftChannelPlayer.draw(0,0,getWidth(),getHeight());
		fboLeft.end();

	}

}
