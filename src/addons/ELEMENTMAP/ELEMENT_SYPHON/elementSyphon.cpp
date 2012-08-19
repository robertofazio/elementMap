#include "elementSyphon.h"

// we are considering spyhon is not stereo !! it could be in a top/bottom or anaglyph state ?

//-----------------------------------------------------------------
elementSyphon::elementSyphon()
{
	this->setPosition(0,0,0);
}

//-----------------------------------------------------------------
void elementSyphon::setup(string _applicationName,string _serverName,int _width,int _height,int _posX, int _posY,string _name, bool _isWarpable)
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

	this->init(3,_width,_height,GL_RGBA,_name,false, _isWarpable);	

}


void elementSyphon::addFX(int type)       // Mauro
{
    switch(type)
    {
        case ELEMENT_FX_MASK:            
            newEffect.init(ELEMENT_FX_MASK, syphonClient.getTextureReference());
            effects.push_back(&newEffect);
            break;
    }    
}

/*
void elementSyphon::drawPreview(int x, int y, int w, int h)
{
//    for(int a = 0; a < effects.size(); a++)
//    {
//        if(effects[a]->getIsActive())
//        {
//            applyFX();
//            effects[a]->finalFbo.draw(x, y, w, h);
//        }
//        else
//            effects[a]->getLeftTexture().draw(x, y, w, h);
//        if(effects[a]->getGUIVisible())
//        {
//            effects[a]->drawGUI(x, y, w, h);
//        }
//    }
  //  if(effects.size() == 0)
    //    drawLeft(x,y, w, h);
}
 */

void elementSyphon::applyFX()
{
    for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
        {
            //effects[a]->getLeftTexture() = syphonClient.getTextureReference();
            effects[a]->applyFX(syphonClient.getTextureReference());
        }
    }
}
/*
//-----------------------------------------------------------------
void elementSyphon::drawGraphic(int x, int y, int w, int h)
{
	this->drawLeft(x,y,w,h);
}
*/

//-----------------------------------------------------------------
void elementSyphon::update()
{
    if (isWarpable)  warper.updateCoordinates();
}
//-----------------------------------------------------------------
void elementSyphon::drawLeft(int x, int y, int w, int h)
{

    fboLeft.begin();
    if (isWarpable) warper.draw(getLeftTexture());
    else syphonClient.draw(x,y,w,h);	
    fboLeft.end();
    
    ofPushStyle();
    int _opacity=int(ofMap(getOpacity(), 0, 1, 0, 255));
    ofSetColor(255, 255, 255,_opacity);
    fboLeft.draw(x,y,w,h);
    ofPopStyle();        

}



//-----------------------------------------------------------------
void elementSyphon::drawRight(int x, int y, int w, int h)
{
    fboRight.begin();
    if (isWarpable) warper.draw(getLeftTexture());
    else syphonClient.draw(x,y,w,h);	
    fboRight.end();
    
    ofPushStyle();
    int _opacity=int(ofMap(getOpacity(), 0, 1, 0, 255));
    ofSetColor(255, 255, 255,_opacity);
    fboRight.draw(x,y,w,h);
    ofPopStyle();        
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



