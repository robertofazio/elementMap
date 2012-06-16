#include "elementMixer.h"

//#include "elementSyphon.h"


// TODO ::
// add support for alpha values on the PSmode mixer

// find a solution for the drawQuadGeometry . 
// now it's working because we're working an 
// image and we're discarting the tex0 uniform in shader.


//-----------------------------------------------------------------
void elementMixer::setup(int _width, int _height, int _stereoMode,vector<element*>* _elements,int* _elementsOrder,int _posX, int _posY,string _name)
{
	setOutputStereoMode(_stereoMode);
	
	xPos = _posX;
	yPos = _posY;

	this->init(5,_width,_height,GL_RGBA,_name);
	sceneElements = *_elements;
	elementsOrder = _elementsOrder;
	elementsBlendModes = new int[sceneElements.size()];
    blendMode = 0;	
	shader.load("./shaders/pssh");
	blacktexture.loadImage("./images/blackTexture.jpg");
	
	
}


//-----------------------------------------------------------------
void elementMixer::update()
{
	for(int i=0;i<sceneElements.size();i++)
	{
		sceneElements[i]->update();
	}		
}

//-----------------------------------------------------------------
void elementMixer::drawIntoFbo(bool _drawMonoOrStereo)
{
	if(isActive)
	{

		setDrawInStereo(_drawMonoOrStereo);

		// we make all the elements to draw them selves into their Fbo 
		for(int i=0;i<sceneElements.size();i++)
		{
			if(sceneElements[i]->getDrawInStereo()) sceneElements[i]->drawIntoFbo(true);
			else sceneElements[i]->drawIntoFbo(false);
			if(i<sceneElements.size()-1)
			{
				elementsBlendModes[i] = sceneElements[i]->getBlendingMode();
			}
		}

		// and now we paint all the layers with the PSblend mixer shader 
		// once per channel left and right 
		// here the left :
		//////////////////////////
		fboLeft.begin();
		shader.begin();
		for(int i=0;i<sceneElements.size();i++)
		{
			string texName = string("tex"+ofToString(i));
			shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->fboLeft.getTextureReference(), i+1);
		}
		
		//elementsBlendModes[0]=3;
		//elementsBlendModes[1]=3;
		//elementsBlendModes[2]=9;
		
		
		shader.setUniform1iv("applyModes",elementsBlendModes, sceneElements.size()-1);
		ofEnableAlphaBlending();
		
		//sceneElements[0]->fboLeft.draw(0,0,1920,1080); 
		
		drawQuadGeometry();
		//sceneElements[0]->fboLeft.draw(0,0);
		
		ofDisableAlphaBlending();
		
		shader.end();        
		fboLeft.end();
		
		// here the right :
		//////////////////////////
		
		fboRight.begin();
		shader.begin();
		shader.setUniform1i("numOfTextures",sceneElements.size());
		for(int i=0;i<sceneElements.size();i++)
		{
			string texName = string("tex"+ofToString(i));

			if(sceneElements[elementsOrder[i]]->getDrawInStereo())
			{
				shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->fboRight.getTextureReference(), i+1);
			}
			else
			{
				shader.setUniformTexture(texName.c_str(), sceneElements[elementsOrder[i]]->fboLeft.getTextureReference(), i+1);
			}
		}
		
		shader.setUniform1iv("applyModes",elementsBlendModes, sceneElements.size()-1);
		ofEnableAlphaBlending();
		//	sceneElements[0]->fboRight.draw(0,0,1920,1080); 
		
		// this function has to paint the quad geometry as 
		// the texture comes from the shader... 
		// now it's not implemented but using a workrround
		drawQuadGeometry();
		//sceneElements[0]->fboRight.draw(0,0);

		ofDisableAlphaBlending();
		shader.end();
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
					glDrawBuffer(GL_BACK_LEFT);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glDrawBuffer(GL_BACK_RIGHT);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
					
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
					glDrawBuffer(GL_BACK_LEFT);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glDrawBuffer(GL_BACK_RIGHT);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
					
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
					glDrawBuffer(GL_BACK);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
					//ofSetupGraphicDefaults();	
					setOpacityColor();
					fboLeft.draw(_x,_y,_width,_height);
				
					
				break;
			case ELM_STEREO_ANAGLYPH:
				// MONO PIPELINE ... works?
				// Clear buffer
				glDrawBuffer(GL_BACK);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
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

//-----------------------------------------------------------------
void elementMixer::drawInfo()
{
	
	if(getIsStereo())
	{
		ofSetColor(0,255,0);
		ofDrawBitmapString("GL_STEREO",ofGetWidth()-100,ofGetHeight()-100);
	}
	else 
	{
		ofSetColor(255,0,0);
		ofDrawBitmapString("no GL_STEREO",ofGetWidth()-100,ofGetHeight()-100);
	}
	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString(ofGetFrameRate()),ofGetWidth()-100,ofGetHeight()-80);

}

//-----------------------------------------------------------------
void elementMixer::drawQuadGeometry()
{
	// this is a worarround. using a blacktexture 
	blacktexture.draw(0,0,1920,1080);
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