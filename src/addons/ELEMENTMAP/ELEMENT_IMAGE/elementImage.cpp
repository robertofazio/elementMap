#include "elementImage.h"



//-----------------------------------------------------------------
elementImage::elementImage()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementImage::setup(string _leftImage, string _rightImage, bool _isStereo)
{
	setIsStereo(_isStereo);
	setDrawInStereo(_isStereo);
	
	if(getIsStereo())
	{
		rightImage.loadImage(_rightImage);
	}
	leftImage.loadImage(_leftImage);
	this->init(2,int(leftImage.getWidth()),int(leftImage.getHeight()),GL_RGBA);	
}

//-----------------------------------------------------------------
void elementImage::update()
{

}

//-----------------------------------------------------------------
void elementImage::drawIntoFbo(bool _drawMonoOrStereo)
{

	if(getDrawInStereo())
	{
		
		if(!getSwapLeftRight())
		{
			// stereo
			fboLeft.begin();
			ofBackground(0,0,0,0);
			ofSetColor(255);
			leftImage.draw(0,0,getWidth(),getHeight());
			fboLeft.end();
			
			//----------
			
			fboRight.begin();
			ofBackground(0,0,0,0);
			ofSetColor(255);
			rightImage.draw(0,0,getWidth(),getHeight());
			fboRight.end();
			
		}
		else
		{
			// stereo swapped
			fboLeft.begin();
			ofBackground(0,0,0,0);
			ofSetColor(255);
			rightImage.draw(0,0,getWidth(),getHeight());
			fboLeft.end();
			
			//----------
			
			fboRight.begin();
			ofBackground(0,0,0,0);
			ofSetColor(255);
			leftImage.draw(0,0,getWidth(),getHeight());
			fboRight.end();
			
		}
	}
	else 
	{
		// mono
		fboLeft.begin();
		ofBackground(0,0,0,0);
		leftImage.draw(0,0,getWidth(),getHeight());
		fboLeft.end();
	}

}
