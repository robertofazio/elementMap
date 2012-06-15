#include "elementSyphon.h"



//-----------------------------------------------------------------
elementSyphon::elementSyphon()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementSyphon::setup(string _applicationName,string _serverName,int _width,int _height)
{
	setIsStereo(false);
	setDrawInStereo(false);

	syphonClient.setApplicationName(_applicationName);
	syphonClient.setServerName(_serverName);
	syphonClient.setup();
	
	// seems like syphonClient can't read w and h ?¿
	//this->init(int(syphonClient.getWidth()),int(syphonClient.getHeight()),GL_RGBA);	
	this->init(3,_width,_height,GL_RGBA);	

}


//-----------------------------------------------------------------
void elementSyphon::drawIntoFbo(bool _drawMonoOrStereo)
{

	if(!hide)
	{

		fboLeft.begin();
		// we need to clear the fbo in order to keep alpha background clean ?¿
		// ofBackgrouns works better ?
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		ofBackground(0,0,0,0);
		setOpacityColor();
		syphonClient.draw(0,0,getWidth(),getHeight());

		fboLeft.end();
	}
	
}
