#include "elementVideo.h"


//-----------------------------------------------------------------
elementVideo::elementVideo()
{
	this->setPosition(0,0,0);
}


//-----------------------------------------------------------------
void elementVideo::openLeft(string _path) {
    
    leftChannelPlayer.close();
    leftChannelPath=_path;
    leftChannelPlayer.loadMovie(leftChannelPath);
    leftChannelPlayer.play();
    leftChannelPlayer.stop();
}


//-----------------------------------------------------------------
void elementVideo::openRight(string _path) {
    
    rightChannelPlayer.close();
    rightChannelPath=_path;
    rightChannelPlayer.loadMovie(rightChannelPath);
    rightChannelPlayer.play();
    rightChannelPlayer.stop();

}


//-----------------------------------------------------------------
void elementVideo::setup(string _leftChannel, string _rightChannel, int _width, int _height, int inputType,int _xPos, int _yPos,string _name, bool _isWarpable)
{
    
	// UI params
	xPos = _xPos;
	yPos = _yPos;
    
    //file ref
    leftChannelPath=_leftChannel;
    rightChannelPath=_rightChannel;

    leftChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);
    rightChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);


    leftChannelPlayer.loadMovie(leftChannelPath);
        
	this->init(1,int(_width),int(_height),leftChannelPlayer.getTextureReference().getTextureData().glTypeInternal,_name,inputType, _isWarpable);

    if(getElementInputType()==ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        if (rightChannelPath!="") rightChannelPlayer.loadMovie(rightChannelPath);   
        else rightChannelPlayer.loadMovie(leftChannelPath);
    }
        leftChannelPlayer.loadMovie(leftChannelPath);
    
    if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempLeft.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight());
        lT.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight(), GL_RGBA);
        tempRight.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight());
        rT.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight(), GL_RGBA);
    }
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempLeft.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5);
        lT.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5, GL_RGBA);
        tempRight.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5);
        rT.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5, GL_RGBA);
    }

    mute    =    false;
    volume  =    1;
    leftChannelPlayer.setVolume(volume);
        
}

//-----------------------------------------------------------------
void elementVideo::update()
{
    
    if (isWarpable) warper.updateCoordinates();

	leftChannelPlayer.idleMovie();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.idleMovie();
    
    if (mute) leftChannelPlayer.setVolume(0);
    else leftChannelPlayer.setVolume(volume);
    
}

//-----------------------------------------------------------------
void elementVideo::drawLeft(int x, int y, int w, int h)
{
    
    fboLeft.begin();
    if (isWarpable) warper.draw(getLeftTexture());
    else leftChannelPlayer.draw(x,y,w,h);	
    fboLeft.end();
    
    ofPushStyle();
    ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
    fboLeft.draw(x,y,w,h);
    ofPopStyle();        
}

//-----------------------------------------------------------------
void elementVideo::drawRight(int x, int y, int w, int h)
{
    fboRight.begin();
    if (isWarpable) warper.draw(getRightTexture());
    else rightChannelPlayer.draw(x,y,w,h);	
    fboRight.end();
    
    ofPushStyle();
    ofSetColor(getRed(), getGreen(), getBlue(),int(ofMap(getOpacity(), 0, 1, 0, 255)));
    fboRight.draw(x,y,w,h);
    ofPopStyle();
}


//-----------------------------------------------------------------
ofTexture& elementVideo::getLeftTexture()
{
    
    // se sono in mono o con due canali separati, prendo semplicemente la texture dallo stream video:
    if (getElementInputType() == ELM_INPUT_MONO || getElementInputType() == ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        return (leftChannelPlayer.getTextureReference());	
    }
    //invece se il video è in formato left/right estraggo la parte sinistra:
    else if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempLeft.begin();
        ofClear(0,0,0,0);
        leftChannelPlayer.draw(0,0,leftChannelPlayer.getWidth(),leftChannelPlayer.getHeight());
        tempLeft.end();
    
        lT.clear();
        lT=tempLeft.getTextureReference();
    
        return (lT);
    }
    //infine, se il video è in formato top/bottom estraggo la parte superiore:
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempLeft.begin();
        ofClear(0,0,0,0);
        leftChannelPlayer.draw(0,0,leftChannelPlayer.getWidth(),leftChannelPlayer.getHeight());
        tempLeft.end();
        
        lT.clear();
        lT=tempLeft.getTextureReference();
        
        return (lT);
    }
        

    
}

//-----------------------------------------------------------------
ofTexture& elementVideo::getRightTexture()
{
    
    // se sono in mono questo non dovrebbe esistere, ma lascio la preview se è caricato qualcosa:
    if (getElementInputType() == ELM_INPUT_MONO)
    {
        if (rightChannelPath!="") return (rightChannelPlayer.getTextureReference());	
        // sennò "svuoto" l'fbo relativo e lascio tutto nero
        else 
        {
            tempRight.begin();
//            ofClear(0,0,0,0);
            ofPushStyle();
            ofSetColor(ofColor :: black);
            ofRect(0, 0, tempRight.getWidth(), tempRight.getHeight());
            ofPopStyle();
            tempRight.end();

            rT.clear();
            return (rT);
        }
    }
    // se sono con due canali separati, prendo semplicemente la texture dallo stream video:
    else if (getElementInputType() == ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        return (rightChannelPlayer.getTextureReference());	
    }
    //invece se il video è in formato left/right estraggo la parte destra:
    else if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        tempRight.begin();
        ofClear(0,0,0,0);
        leftChannelPlayer.draw(-leftChannelPlayer.getWidth()*.5,0,leftChannelPlayer.getWidth(),leftChannelPlayer.getHeight());
        tempRight.end();
        
        rT.clear();
        rT=tempRight.getTextureReference();
        
        return (rT);
    }
    //infine, se il video è in formato top/bottom estraggo la parte inferiore:
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        tempRight.begin();
        ofClear(0,0,0,0);
        leftChannelPlayer.draw(0, -leftChannelPlayer.getHeight(),leftChannelPlayer.getWidth(),leftChannelPlayer.getHeight());
        tempRight.end();
        
        rT.clear();
        rT=tempRight.getTextureReference();
        
        return (rT);
    }
}

// PLAYER UTILITIES //

//-----------------------------------------------------------------
void elementVideo::element_videoPlay(int _speed)
{
    leftChannelPlayer.setSpeed(_speed);
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.setSpeed(_speed);
    leftChannelPlayer.play();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.play();
}

//-----------------------------------------------------------------
void elementVideo::element_videoPause()
{
    leftChannelPlayer.stop();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.stop();   
}

//-----------------------------------------------------------------
void elementVideo::element_videoStop()
{
    leftChannelPlayer.play();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.play();
    leftChannelPlayer.setFrame(0);
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.setFrame(0);
    leftChannelPlayer.stop();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.stop();
}

//-----------------------------------------------------------------
void elementVideo::element_frameAvanti()
{
    leftChannelPlayer.play();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.play();
    leftChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame()+1);
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame());
    leftChannelPlayer.stop();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.stop();    
}

//-----------------------------------------------------------------
void elementVideo::element_frameIndietro()
{
    leftChannelPlayer.play();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.play();
    leftChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame()-1);
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.setFrame(leftChannelPlayer.getCurrentFrame());
    leftChannelPlayer.stop();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.stop();    
    
}

//-----------------------------------------------------------------
void elementVideo::element_goToFrame(int _frame)
{
    leftChannelPlayer.play();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.play();
    leftChannelPlayer.setFrame(_frame);
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.setFrame(_frame);
    leftChannelPlayer.stop();
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.stop();    
    
}

//-----------------------------------------------------------------
void elementVideo::element_setLoop(ofLoopType _loop)
{
    leftChannelPlayer.setLoopState(_loop);
    if (getElementType() == ELM_INPUT_STEREO_TWO_CHANNEL) rightChannelPlayer.setLoopState(_loop);
}

//-----------------------------------------------------------------
void elementVideo::element_toggleSound()
{
    mute=!mute;
}

//-----------------------------------------------------------------
void elementVideo::element_soundVolume(float _volume)
{
    volume=_volume;    
}



//-----------------------------------------------------------------
void elementVideo::setElementInputType(int _inType)
{
    inputType=_inType;
    

    if (getElementInputType() == ELM_INPUT_MONO)
    {
        leftChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);
        rightChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);

        rightChannelPlayer.close();

    }
    else if (getElementInputType() == ELM_INPUT_STEREO_TWO_CHANNEL)
    {
        leftChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);
        rightChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);
        if (rightChannelPath!="") rightChannelPlayer.loadMovie(rightChannelPath);   
        else ofSystemAlertDialog("ERROR: this mode requires TWO separate videos!");        
        

    }
    if (getElementInputType() == ELM_INPUT_STEREO_LEFTRIGHT)
    {
        leftChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);
        rightChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);

        rightChannelPlayer.close();

        //prepare left:
        tempLeft.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight());
        lT.clear();
        lT.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight(), GL_RGBA);

        //prepare right:
        tempRight.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight());
        rT.clear();
        rT.allocate(leftChannelPlayer.getWidth()*.5, leftChannelPlayer.getHeight(), GL_RGBA);

    }
    else if (getElementInputType() == ELM_INPUT_STEREO_TOPBOTTOM)
    {
        leftChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);
        rightChannelPlayer.setPixelFormat(OF_PIXELS_RGBA);

        rightChannelPlayer.close();
        //prepare left:
        tempLeft.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5);
        lT.clear();
        lT.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5, GL_RGBA);
        
        //prepare right:
        tempRight.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5);
        rT.clear();
        rT.allocate(leftChannelPlayer.getWidth(), leftChannelPlayer.getHeight()*.5, GL_RGBA);        
    }
    
//    cout << "width: " << parentElement->warper.width<< endl;
//    cout << "text.getWidth: " << parentElement->warper.text.getWidth() << endl;

//    parentElement->warper.createGrid(parentElement->warper.xRes, parentElement->warper.yRes);
//    parentElement->warper.updateCoordinates();
//    parentElement->warper.createGrid(parentElement->warper.xRes, parentElement->warper.yRes);
    
    
}




