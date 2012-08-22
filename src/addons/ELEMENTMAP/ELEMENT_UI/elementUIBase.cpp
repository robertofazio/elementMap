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
        
	blendingNames.push_back("Disabled");
//	blendingNames.push_back("Alpha");
	blendingNames.push_back("Add");
	blendingNames.push_back("Subtract");
	blendingNames.push_back("Multiply");
//	blendingNames.push_back("Screen");
	
    outputModesNames.push_back("ANAGLYPH");
    outputModesNames.push_back("MONO");
    outputModesNames.push_back("OPENGL");    
    
	
}
//--------------------------------------------------------------
void elementUIBase::setupUI(element* _parentElement)
{
	printf("setupUI %d %d\n",xPos,yPos);
	parentElement =  _parentElement;
	
    int type = parentElement->getElementType();    
    
    //interfaccia per gli element "generici" (tutti tranne il mixer)
    if (type!=ELEMENT_MIXER)
    {
     
        UI = new ofxUICanvas(xPos,yPos, 520, 160);
        
        ofColor colorBack;
        colorBack.r = 0;
        colorBack.g = 255;
        colorBack.b = 206;
        colorBack.a = 10;
        
        UI->setColorBack(colorBack);
        UI->setDrawBack(true);
        UI->setDrawOutline(false);
        UI->setDrawWidgetPadding(true);
        UI->setFontSize(OFX_UI_FONT_MEDIUM, 8);
        UI->setFontSize(OFX_UI_FONT_SMALL, 6);
        UI->setPadding(2);
        
        int posY=10;

        
        // prima colonna: isActive, Mono/Stereo
        UI->addWidget(new ofxUILabelToggle(5, posY, 120,16,parentElement->getIsActive(),"isActive", OFX_UI_FONT_SMALL));
        if(this->isStereo) UI->addWidget(new ofxUILabelToggle(5,posY+=20, 120, 16,parentElement->getDrawInStereo(),"Stereoscopic", OFX_UI_FONT_SMALL));
        if(parentElement->isWarpable)
        {
            UI->addWidget(new ofxUIToggle(5, posY+=20, 16,16,false,"Quad Warping"));
            UI->addWidget(new ofxUIToggle(5, posY+=20, 16,16,false,"Fine Warping"));
            
            UI->addWidget(new ofxUIImageButton(25, posY+=20, 16, 16, true, "./GUI/images/meno.png", "xGridDecrease"));
            UI->addWidget(new ofxUIImageButton(46, posY, 16, 16, true, "./GUI/images/piu.png", "xGridIncrease"));
            UI->addWidget(new ofxUILabel(68, posY+4, "xGrid", "xGrid", OFX_UI_FONT_SMALL));
            
            UI->addWidget(new ofxUIImageButton(25, posY+=20, 16, 16, true, "./GUI/images/meno.png", "yGridDecrease"));
            UI->addWidget(new ofxUIImageButton(46, posY, 16, 16, true, "./GUI/images/piu.png", "yGridIncrease"));
            UI->addWidget(new ofxUILabel(68, posY+4, "yGrid", "yGrid", OFX_UI_FONT_SMALL));
            
            UI->addWidget(new ofxUILabelButton(5, posY+=20, 55, false, "reset warp", OFX_UI_FONT_SMALL));
            UI->addWidget(new ofxUILabelButton(65, posY, 55, false, "reset grid", OFX_UI_FONT_SMALL));
            
            
        }
        
        
        posY=10;
        
        //seconda colonna: opacità e componenti r,g,b
        UI->addWidget(new ofxUIMinimalSlider(150, posY, 100,10,0.0,1.0,parentElement->getOpacity() ,"Opacity"));
        UI->addWidget(new ofxUIMinimalSlider(150, posY+=20, 100,10, 0, 255, parentElement->getRed() ,"RED"));
        UI->addWidget(new ofxUIMinimalSlider(150, posY+=20, 100,10, 0, 255, parentElement->getGreen() ,"GREEN"));
        UI->addWidget(new ofxUIMinimalSlider(150, posY+=20, 100,10, 0, 255, parentElement->getBlue() ,"BLUE"));
        UI->addWidget(new ofxUIButton(150, posY+=20, 16, 16, false, "RGB reset"));
        
        posY=10;
        
        //terza colonna: blending mode
        listBlendModes = new ofxUIDropDownList(280, posY, 100, "Blending Mode", blendingNames, OFX_UI_FONT_SMALL);
        listBlendModes->setDrawBack(true);
        listBlendModes->setDrawOutlineHighLight(false);
        listBlendModes->setDrawPaddingOutline(false);
        listBlendModes->setPadding(0);
        listBlendModes->setDrawFill(true);
        listBlendModes->setAutoClose(true);
        UI->addWidget(listBlendModes);
        
        
    }
    
    //but mixer needs output mode selection and many other things :) 
	else if (type==ELEMENT_MIXER)
    {
        UI = new ofxUICanvas(xPos,yPos+450, 600, 140);
        
        ofColor colorBack;
        colorBack.r = 0;
        colorBack.g = 255;
        colorBack.b = 206;
        colorBack.a = 10;
        
        UI->setColorBack(colorBack);
        UI->setDrawBack(true);
        UI->setDrawOutline(false);
        UI->setDrawWidgetPadding(true);
        UI->setFontSize(OFX_UI_FONT_MEDIUM, 8);
        UI->setFontSize(OFX_UI_FONT_SMALL, 6);
        UI->setPadding(2);
        
        int marginLeft = 5;
        int posY = 10;

        
        //parte superiore sinistra: test Pattern e Mixer Stereo toggle
        UI->addWidget(new ofxUILabelToggle(marginLeft, posY, 120,16,false,"Test Pattern", OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUILabelToggle(marginLeft,posY += 20, 120, 16,parentElement->getDrawInStereo(),"Stereoscopic", OFX_UI_FONT_SMALL));
        ofxUISpacer* spacer = new ofxUISpacer(marginLeft, posY+=20, 400, 1);
        spacer->setColorFill(ofColor(0, 255, 206));
        UI->addWidget(spacer);
        
        //parte superiore centrale: opacità
        posY=10;
        UI->addWidget(new ofxUIMinimalSlider(marginLeft+150, posY, 100,16,0.0,1.0,parentElement->getOpacity() ,"Main Opacity"));
        
        //parte superiore destra: output mode
        listOutputModes = new ofxUIDropDownList(marginLeft + 300, posY, "Output Mode", outputModesNames, OFX_UI_FONT_SMALL);
        listOutputModes->setDrawBack(true);
        listOutputModes->setDrawOutlineHighLight(false);
        listOutputModes->setDrawPaddingOutline(false);
        listOutputModes->setPadding( 0 );
        listOutputModes->setDrawFill(true);
        listOutputModes->setAutoClose(true);
        UI->addWidget(listOutputModes);
        
        //parte inferiore: video player
        posY=60;
        UI->addWidget(new ofxUIImageButton(marginLeft, posY, 30, 30, true, "./GUI/images/play.png", "Play"));
        UI->addWidget(new ofxUIImageButton(marginLeft+=35, posY, 30, 30, true, "./GUI/images/pause.png", "Pause"));
        UI->addWidget(new ofxUIImageButton(marginLeft+=35, posY, 30, 30, true, "./GUI/images/stop.png", "Rew"));
        UI->addWidget(new ofxUIImageButton(marginLeft+=35, posY, 30, 30, true, "./GUI/images/play.png", "prevFrame"));
        UI->addWidget(new ofxUIImageButton(marginLeft+=35, posY, 30, 30, true, "./GUI/images/play.png", "nextFrame"));
        UI->addWidget(new ofxUIImageToggle(marginLeft+=35, posY, 30, 30, true, "./GUI/images/loop.png", "Loop"));
        UI->addWidget(new ofxUIImageToggle(marginLeft+=35, posY, 30, 30, true, "./GUI/images/mute.png","Sound on/off"));
        UI->addWidget(new ofxUISlider(marginLeft+=35, posY, 100,16,0,100,50 ,"Sound Volume"));
        
    }


    if (type!=ELEMENT_MIXER) ofAddListener(UI->newGUIEvent,this,&elementUIBase::guiEvent); 

}

//--------------------------------------------------------------
void elementUIBase::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
	if(e.widget->getName()=="Opacity")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setOpacity(slider->getScaledValue());
	}
	else if(e.widget->getName()=="RED")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setRed(int(slider->getScaledValue()));
    }
    else if(e.widget->getName()=="GREEN")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setGreen(int(slider->getScaledValue()));
    }
    else if(e.widget->getName()=="BLUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setBlue(int(slider->getScaledValue()));
    }
    else if(e.widget->getName()=="RGB reset")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        parentElement->setRed(255);
        parentElement->setGreen(255);
        parentElement->setBlue(255);
    }
	
    else if(e.widget->getName()=="isActive")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setIsActive(toggle->getValue());
		if (toggle->getValue()) parentElement->setOpacity(1);
        else parentElement->setOpacity(0);
        
	}
    else if (e.widget->getName()=="Select")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
    }
    
    else if(e.widget->getName()=="isDrawInStereo")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setDrawInStereo(toggle->getValue());
	}
    else if(e.widget->getName()=="Stereoscopic")
        {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        parentElement->setDrawInStereo(toggle->getValue());
        }
    else if( e.widget->getParent()->getName()=="Blending Mode")
	{
            if(name=="Disabled") parentElement->setBlendingMode(OF_BLENDMODE_DISABLED);
            else if(name=="Alpha") parentElement->setBlendingMode(OF_BLENDMODE_ALPHA);
            else if(name=="Add")parentElement->setBlendingMode(OF_BLENDMODE_ADD);
            else if(name=="Subtract")parentElement->setBlendingMode(OF_BLENDMODE_SUBTRACT);
            else if(name=="Multiply")parentElement->setBlendingMode(OF_BLENDMODE_MULTIPLY);
            else if(name=="Screen")parentElement->setBlendingMode(OF_BLENDMODE_SCREEN);
                
	}
	
}


