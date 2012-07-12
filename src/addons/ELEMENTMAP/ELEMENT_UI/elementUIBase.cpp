#include "elementUIBase.h"

// we can include element.h to have it's reference to methods and variables
// because we added a forwarded declaration on the .h
// in james words :
#include "element.h"

//same thing for testSpp, because we point to the mixer element:
#include "testApp.h"

//--------------------------------------------------------------
elementUIBase::elementUIBase()
{
	blendingNames.push_back("Normal");
	blendingNames.push_back("Multiply");
	blendingNames.push_back("Average");
	blendingNames.push_back("Add");
	blendingNames.push_back("Substract");
	blendingNames.push_back("Difference");
	blendingNames.push_back("Negation");
	blendingNames.push_back("Exclusion");
	blendingNames.push_back("Screen");
	blendingNames.push_back("Overlay");
	blendingNames.push_back("SoftLight");
	blendingNames.push_back("HardLight");
	blendingNames.push_back("Dodge");
	blendingNames.push_back("Burn");
	blendingNames.push_back("LinearLight");
	blendingNames.push_back("VividLight");
	blendingNames.push_back("PinLight");
	blendingNames.push_back("HardMix");
	blendingNames.push_back("Reflect");
	blendingNames.push_back("Glow");
	blendingNames.push_back("Phoenix");
	blendingNames.push_back("Hue");
	blendingNames.push_back("Saturation");
	blendingNames.push_back("Bor");
	blendingNames.push_back("Luminosity");
	
    outputModesNames.push_back("ANAGLYPH");
    outputModesNames.push_back("MONO");
//	  outputModesNames.push_back("OPENGL");
//    outputModesNames.push_back("LEFTRIGHT");
//    outputModesNames.push_back("TOPBOTTOM");    

	
}
//--------------------------------------------------------------
void elementUIBase::setupUI(element* _parentElement)
{
	printf("setupUI %d %d\n",xPos,yPos);
	parentElement =  _parentElement;
	
    int type = parentElement->getElementType();    
    
    UI = new ofxUICanvas(xPos,yPos, 1000, 1000);
    
    ofColor colorBack;
    colorBack.r = 200;
    
    UI->setColorBack(colorBack);
    UI->setDrawBack(false);
    UI->setDrawOutline(false);
    UI->setFontSize(OFX_UI_FONT_MEDIUM, 8);
    UI->setFontSize(OFX_UI_FONT_SMALL, 6);
    UI->setPadding(2);
	
    int marginLeft = 5;
    int posY = 9;
    if (type!=5)
    { 
    //  UI->addWidgetDown(new ofxUILabel(parentElement->getElementName(), OFX_UI_FONT_MEDIUM));
    UI->addWidget(new ofxUIToggle(marginLeft, posY, 10,10,parentElement->getIsActive(),"isActive"));
    if(this->isStereo)
        UI->addWidget(new ofxUIToggle(marginLeft,posY += 20, 10, 10,parentElement->getIsActive(),"Mono/Stereo"));
    UI->addWidget(new ofxUIToggle(marginLeft,posY += 20, 10, 10,parentElement->getIsActive(),"Mask"));
    UI->addWidget(new ofxUISlider(marginLeft, 90, 100,10,0.0,1.0,parentElement->getOpacity() ,"Opacity"));
    
    posY = 9;
    marginLeft = 110;
    UI->addWidget(new ofxUIToggle(marginLeft, posY, 10,10,parentElement->getIsActive(),"Quad Warping"));
    UI->addWidget(new ofxUIToggle(marginLeft, posY += 20, 10,10,parentElement->getIsActive(),"Mesh Warping"));
    
        
    //all elements except for mixer need blend modes selection
	  
        marginLeft = 5;
        listBlendModes = new ofxUIDropDownList(marginLeft, 120, 100, "Blending Mode", blendingNames, OFX_UI_FONT_SMALL);
        listBlendModes->setDrawBack(true);
        listBlendModes->setDrawOutlineHighLight(false);
        listBlendModes->setDrawPaddingOutline(false);
        listBlendModes->setPadding(0);
        listBlendModes->setDrawFill(false);
        listBlendModes->setAutoClose(true);
        UI->addWidget(listBlendModes);
    }
    //but mixer needs output mode selection and many other things :) 
	if (type==5)
    {
        marginLeft = 5;
        posY = 450;
        //UI->addWidget(new ofxUIFPS(marginLeft, posY, OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUIToggle(marginLeft, posY += 20, 10,10,parentElement->getIsActive(),"Test Pattern"));
        UI->addWidget(new ofxUIToggle(marginLeft,posY += 20, 10, 10,parentElement->getIsActive(),"Visible"));
        
        posY += 20;
        ofxUISpacer* spacer = new ofxUISpacer(marginLeft, posY, 400, 1);
        spacer->setColorFill(ofColor(0, 255, 206));
        
        UI->addWidget(spacer);
        
        //prima colonna: video
        UI->addWidget(new ofxUIButton(marginLeft,posY += 10, 10, 10,false, "Play"));
        UI->addWidget(new ofxUIButton(marginLeft,posY += 20, 10, 10,false,"Pause"));
//        UI->addWidget(new ofxUIButton(marginLeft,posY += 20, 10, 10,false,"Stop"));
        UI->addWidget(new ofxUIButton(marginLeft,posY += 20, 10, 10,false,"Rew"));
        UI->addWidget(new ofxUIToggle(marginLeft,posY += 20, 10, 10,parentElement->getIsActive(),"Video loop"));
        
        //seconda colonna: audio
        posY -= 90;
        UI->addWidget(new ofxUIToggle(marginLeft + 150, posY+=30, 10, 10, parentElement->getIsActive(),"Sound on/off"));
        UI->addWidget(new ofxUISlider(marginLeft + 150, posY+=20, 100,10,0.0,1.0,parentElement->getOpacity() ,"Sound Volume"));

        
        listOutputModes = new ofxUIDropDownList(marginLeft + 250, posY-=80, "Output Mode", outputModesNames, OFX_UI_FONT_SMALL);
        listOutputModes->setDrawBack(true);
        listOutputModes->setDrawOutlineHighLight(false);
        listOutputModes->setDrawPaddingOutline(false);
        listOutputModes->setPadding( 0 );
        listOutputModes->setDrawFill(true);
        listOutputModes->setAutoClose(true);
        UI->addWidget(listOutputModes);
    }
    
    if (type!=5) ofAddListener(UI->newGUIEvent,this,&elementUIBase::guiEvent); 

}

//--------------------------------------------------------------
void elementUIBase::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
//	cout << "got event from: " << name << endl; 
	
	if(e.widget->getName()=="Opacity")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setOpacity(slider->getScaledValue());
	}
    else if(e.widget->getName()=="Test pattern")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setIsActive(toggle->getValue());
	}
	else if(e.widget->getName()=="isActive")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setIsActive(toggle->getValue());
		if (toggle->getValue()) parentElement->setOpacity(1);
        else parentElement->setOpacity(0);
        
	}
    else if(e.widget->getName()=="isDrawInStereo")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setDrawInStereo(toggle->getValue());
	}
    else if(e.widget->getName()=="Mono/Stereo")
        {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        parentElement->setDrawInStereo(toggle->getValue());
        }
    else if( e.widget->getParent()->getName()=="Blending Mode")
	{
		for(int i=0;i<blendingNames.size();i++)
		{
			if(name==blendingNames[i]) 
			{
				parentElement->setBlendingMode(i);
			}			
		}
	}
    else if( e.widget->getParent()->getName()=="Output Mode")
	{
		for(int i=0;i<outputModesNames.size();i++)
		{
			if(name==outputModesNames[i]) 
			{
                ((testApp*)ofGetAppPtr())->elemMix.setOutputStereoMode(i);
			}			
		}
	}
	
}


