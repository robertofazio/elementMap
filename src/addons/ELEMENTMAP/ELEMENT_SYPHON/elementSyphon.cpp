#include "elementSyphon.h"

// we are considering spyhon is not stereo !! it could be in a top/bottom or anaglyph state ?

//-----------------------------------------------------------------
elementSyphon::elementSyphon()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementSyphon::setup(string _applicationName,string _serverName,int _width,int _height,int _posX, int _posY,string _name)
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

	// seems like syphonClient can't read w and h ?¿
	//this->init(int(syphonClient.getWidth()),int(syphonClient.getHeight()),GL_RGBA);	
	this->init(3,_width,_height,GL_RGBA,_name,false);	

}

//-----------------------------------------------------------------
void elementSyphon::drawLeft(int x, int y, int w, int h)
{
	syphonClient.draw(x,y,w,h);
}


//-----------------------------------------------------------------
void elementSyphon::drawRight(int x, int y, int w, int h)
{
	syphonClient.draw(x,y,w,h);	
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

