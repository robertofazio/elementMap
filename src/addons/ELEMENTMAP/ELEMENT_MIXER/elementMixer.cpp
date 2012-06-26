#include "elementMixer.h"

//#include "elementSyphon.h"

#include "elementVideo.h"

// TODO ::
// add support for alpha values on the PSmode mixer

// find a solution for the drawQuadGeometry . 
// now it's working because we're working an 
// image and we're discarting the tex0 uniform in shader.


//-----------------------------------------------------------------
void elementMixer::setup(int _width, int _height, int _stereoMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name)
{
	setOutputStereoMode(_stereoMode);
	if(outputStereoMode==0)
	{	
		this->setDrawInStereo(true);
		this->setIsStereo(true);
		
	}
	// UI params
	xPos = _posX;
	yPos = _posY;

	this->init(5,_width,_height,GL_RGBA,_name,this->getIsStereo());
	sceneElements = _elements;
	numOfElements = _numOfElements;
	elementsOrder = _elementsOrder;
	elementsBlendModes = new int[numOfElements];
	elementsOpacity = new float[numOfElements];
    blendMode = 0;	
	shader.load("./shaders/pssh");
	blacktexture.loadImage("./images/blackTexture.jpg");
	
	useNoShader = false;
	
}


//-----------------------------------------------------------------
void elementMixer::update()
{
	for(int i=0;i<numOfElements;i++)
	{
		sceneElements[i]->update();
	}		
}

void elementMixer::applyFX()
{
 for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
            effects[a]->applyFX();
    }
}


void elementMixer::addFX(int type)       // Mauro
{
    switch(type)
    {
        case ELEMENT_FX_MASK:            
            newEffect.init(ELEMENT_FX_MASK, fboLeft.getTextureReference());
            effects.push_back(&newEffect);
            break;
    }    
}

//-----------------------------------------------------------------
void elementMixer::drawIntoFbo(bool _drawMonoOrStereo)
{
	if(isActive)
	{

		setDrawInStereo(_drawMonoOrStereo);

		// KO :: we make all the elements to draw them selves into their Fbo 
		// and compile the blend and opacities (OPTIMIZABLE?)
		for(int i=0;i<numOfElements;i++)
		{
			// I DEACTIVATED FBO INTO ELEMENTS !!!
			////////////////////////////////////
			//if(sceneElements[i]->getDrawInStereo()) sceneElements[i]->drawIntoFbo(true);
			//else sceneElements[i]->drawIntoFbo(false);
			if(i<numOfElements-1)
			{
				elementsBlendModes[i] = sceneElements[elementsOrder[i]]->getBlendingMode();
				
			}
			elementsOpacity[i] = sceneElements[elementsOrder[i]]->getOpacity();
		}

		// and now we paint all the layers with the PSblend mixer shader 
		// once per channel left and right 
		
		//////////////////////////
		// here the left :
		//////////////////////////
		fboLeft.begin();

		if(useNoShader)
		{
			// performance test 
			sceneElements[0]->drawLeft(0,0,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
			sceneElements[1]->drawLeft(0,384,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
			sceneElements[2]->drawLeft(512,384,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
			sceneElements[3]->drawLeft(512,0,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
		}
		else 
		{
			shader.begin();
			for(int i=0;i<numOfElements;i++)
			{
				string texName = string("tex"+ofToString(i));
				//printf("L%d :: %s \n",i,sceneElements[elementsOrder[i]]->getElementName().c_str());
              //  if(sceneElements[elementsOrder[i]]->effects.size() == 0)
                    shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->getLeftTexture(), i+1);
               // else
                 //  shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->effects[0]->finalFbo.getTextureReference(), i+1);
			}
					
			shader.setUniform1fv("opacities", elementsOpacity,numOfElements);
			shader.setUniform1iv("applyModes",elementsBlendModes, numOfElements-1);

			ofEnableAlphaBlending();
			// this function has to paint the quad geometry as 
			// the texture comes from the shader... 
			// now it's not implemented but using a workrround
			drawQuadGeometry();
			ofDisableAlphaBlending();

			shader.end();   
		}
		fboLeft.end();
		
		//////////////////////////
		// here the right :
		//////////////////////////
		
		fboRight.begin();

		if(useNoShader)
		{
			// performance test 
			sceneElements[0]->drawRight(0,0,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
			sceneElements[1]->drawRight(0,384,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
			sceneElements[2]->drawLeft(512,384,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
			sceneElements[3]->drawRight(512,0,sceneElements[0]->getWidth()/2,sceneElements[0]->getHeight()/2);
		}
		else 
		{

			shader.begin();
			shader.setUniform1i("numOfTextures",numOfElements);
			for(int i=0;i<numOfElements;i++)
			{
				string texName = string("tex"+ofToString(i));

				if(sceneElements[elementsOrder[i]]->getDrawInStereo())
				{
					//printf("R%d :: %s \n",i,sceneElements[elementsOrder[i]]->getElementName().c_str());
					shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->getRightTexture(), i+1);
				}
				else
				{
					//printf("R%d :: %s \n",i,sceneElements[elementsOrder[i]]->getElementName().c_str());
					shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->getLeftTexture(), i+1);
				}
			}
			
			shader.setUniform1iv("applyModes",elementsBlendModes, numOfElements-1);
			
			ofEnableAlphaBlending();
			// this function has to paint the quad geometry as 
			// the texture comes from the shader... 
			// now it's not implemented but using a workrround
			drawQuadGeometry();
			ofDisableAlphaBlending();
			
			shader.end();
		}
		fboRight.end();

	}
	
}

//-----------------------------------------------------------------
void elementMixer::drawOutput(int _x, int _y,int _width, int _height)
{
	if(isActive)
	{

		switch (outputStereoMode) 
		{
			case ELM_STEREO_OPENGL:
				
				if(!getSwapLeftRight())
				{
					// STEREO PIPELINE
					// Clear the buffers 
					//glDrawBuffer(GL_BACK_LEFT);
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					//glDrawBuffer(GL_BACK_RIGHT);
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
					
					// left
					glDrawBuffer(GL_BACK_LEFT);	
					//ofSetupGraphicDefaults();
					setOpacityColor();
					fboLeft.draw(_x,_y,_width,_height);
					
					// right
					glDrawBuffer(GL_BACK_RIGHT);
					//ofSetupGraphicDefaults();
					setOpacityColor();
					if(getDrawInStereo()) fboRight.draw(_x,_y,_width,_height);
					else fboLeft.draw(_x,_y,_width,_height);
				}
				else 
				{
					// STEREO PIPELINE SWAPPED
					// Clear the buffers 
					//glDrawBuffer(GL_BACK_LEFT);
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					//glDrawBuffer(GL_BACK_RIGHT);
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
					
					// left
					glDrawBuffer(GL_BACK_LEFT);	
					//ofSetupGraphicDefaults();	
					setOpacityColor();
					fboRight.draw(_x,_y,_width,_height);
					
					// right
					glDrawBuffer(GL_BACK_RIGHT);
					//ofSetupGraphicDefaults();
					setOpacityColor();
					if(getDrawInStereo()) fboLeft.draw(_x,_y,_width,_height);
					else fboLeft.draw(_x,_y,_width,_height);
				}

				break;
				
			case ELM_STEREO_MONO:
					// MONO PIPELINE ... works?
					// Clear buffer
					//glDrawBuffer(GL_BACK);
					//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
					//ofSetupGraphicDefaults();	
					setOpacityColor();
					fboLeft.draw(_x,_y,_width,_height);
				
					
				break;
			case ELM_STEREO_ANAGLYPH:
				// MONO PIPELINE ... works?
				// Clear buffer
				//glDrawBuffer(GL_BACK);
				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
				//ofSetupGraphicDefaults();	
				
				// left
				glColorMask(true, false, false, false);
				//ofSetupGraphicDefaults();	
				setOpacityColor();
				fboLeft.draw(_x,_y,_width,_height);
				
				// right
				glColorMask(false, true, true, false);
				//ofSetupGraphicDefaults();
				setOpacityColor();
				fboRight.draw(_x,_y,_width,_height);
				
				glColorMask(true, true, true, true);
				
				break;
			default:
				break;
		}
		if(getIsStereo())
		{
		}
		else 
		{
		}

		drawInfo();
	}
}

void elementMixer::drawPreview(int x, int y, int w, int h)
{
    for(int a = 0; a < effects.size(); a++)
    {
        if(effects[a]->getIsActive())
        {
            applyFX();
            setOpacityColor();
      //      fboLeft.draw(x, y, w, h);
            effects[a]->finalFbo.draw(x, y, w, h);
        }
        else
            effects[a]->getLeftTexture().draw(x, y, w, h);
        if(effects[a]->getGUIVisible())
        {
            effects[a]->drawGUI(x, y, w, h);
        }
    }

    if(effects.size() == 0)
        {
        setOpacityColor();
        fboLeft.draw(x, y, w, h);
        }

}

//-----------------------------------------------------------------
void elementMixer::drawInfo()
{
    
    ofPushStyle();
	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-100,ofGetHeight()-80);
    
	ofSetColor(0,255,0);

	switch (outputStereoMode) 
	{
		case ELM_STEREO_OPENGL:
			ofDrawBitmapString("GL_STEREO",ofGetWidth()-100,ofGetHeight()-100);
			break;
			
		case ELM_STEREO_MONO:
			ofDrawBitmapString("MONO",ofGetWidth()-100,ofGetHeight()-100);
			break;
		case ELM_STEREO_ANAGLYPH:
			ofDrawBitmapString("ANAGLYPH",ofGetWidth()-100,ofGetHeight()-100);
			break;
		default:
			break;
	}
    ofPopStyle();

}

//-----------------------------------------------------------------
void elementMixer::drawQuadGeometry()
{
	// this is a worarround. using a blacktexture 
	blacktexture.draw(0,0,elementWidth,elementHeight);
	/*
	glBegin(GL_QUADS);
	
	glVertex3i(0,0,0);
	glTexCoord2f(0.0,0.0);
	
	glVertex3i(1920,0,0);
	glTexCoord2f(1.0,0.0);
	
	glVertex3i(1920,1080,0);
	glTexCoord2f(1.0,1.0);
	
	glVertex3i(0,1080,0);
	glTexCoord2f(0.0,1.0);
	
	glEnd();

	ofSetColor(255,0,0);
	ofCircle(1920/2,1080/2,100);
	 */
}

//-----------------------------------------------------------------
void elementMixer::setOutputStereoMode(int _stereoMode)
{
	outputStereoMode = _stereoMode;
	
	if((outputStereoMode==ELM_STEREO_ANAGLYPH)||(outputStereoMode==ELM_STEREO_MONO))
	{
		setIsStereo(false);
	}
	else
	{ 
		setIsStereo(true);
	}
	
}
			   

//-----------------------------------------------------------------
void elementMixer::updateOpacity()
{
	//elementsOpacity[0] = *sceneElements[0]->getOpacity();
	
}


//void elementMixer::drawLeft(int x, int y, int w, int h)
//{
//}
//
//
//void elementMixer::drawRight(int x, int y, int w, int h)
//{
//}
