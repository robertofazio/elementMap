#include "elementImage.h"



//-----------------------------------------------------------------
elementImage::elementImage()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementImage::setup(string _leftImage, string _rightImage, bool _isStereo,int _posX, int _posY,string _name)
{
	setIsStereo(_isStereo);
	setDrawInStereo(_isStereo);
	
	if(getIsStereo())
	{
		rightImage.loadImage(_rightImage);
	}
	leftImage.loadImage(_leftImage);
	
	xPos = _posX;
	yPos = _posY;

	this->init(2,int(leftImage.getWidth()),int(leftImage.getHeight()),GL_RGBA,_name);	
}

//-----------------------------------------------------------------
void elementImage::update()
{

}

//-----------------------------------------------------------------
void elementImage::drawIntoFbo(bool _drawMonoOrStereo)
{

	if(isActive)
	{
		// update the clear state of the object 
		if(isClear) isClear=false;
		
		this->setDrawInStereo(_drawMonoOrStereo);

		if(getDrawInStereo())
		{
			
			if(!getSwapLeftRight())
			{
				// stereo
				fboLeft.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				leftImage.draw(0,0,getWidth(),getHeight());
				fboLeft.end();
				
				//----------
				
				fboRight.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				rightImage.draw(0,0,getWidth(),getHeight());
				fboRight.end();
				
			}
			else
			{
				// stereo swapped
				fboLeft.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				rightImage.draw(0,0,getWidth(),getHeight());
				fboLeft.end();
				
				//----------
				
				fboRight.begin();
				ofBackground(0,0,0,0);
				setOpacityColor();
				leftImage.draw(0,0,getWidth(),getHeight());
				fboRight.end();
				
			}
		}
		else 
		{
			// mono
			fboLeft.begin();
			ofBackground(0,0,0,0);
			setOpacityColor();
			leftImage.draw(0,0,getWidth(),getHeight());
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
