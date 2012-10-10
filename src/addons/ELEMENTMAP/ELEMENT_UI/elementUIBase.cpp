#include "elementUIBase.h"

// we can include element.h to have it's reference to methods and variables
// because we added a forwarded declaration on the .h
// in james words :
#include "element.h"

//same thing for testApp, because we point to the mixer element:
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
    outputModesNames.push_back("ACTIVE STEREO");
    
    inputTypeNames.push_back("MONO");
    inputTypeNames.push_back("2CHANNEL");
    inputTypeNames.push_back("LEFTRIGHT");
    inputTypeNames.push_back("TOPBOTTOM");
	
}
//--------------------------------------------------------------
void elementUIBase::setupUI(element* _parentElement)
{
	printf("setupUI %d %d\n",xPos,yPos);
	parentElement =  _parentElement;
	
    type = parentElement->getElementType();    
    
    //interfaccia per gli element "generici" (tutti tranne il mixer)
    if (type!=ELEMENT_MIXER)
    {
     
        UI = new ofxUICanvas(xPos,yPos, 630, 160);
        
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
        GUI_isActive = new ofxUILabelToggle(115, posY, 120,16,parentElement->getIsActive(),"isActive", OFX_UI_FONT_SMALL); 
        UI->addWidget(GUI_isActive);
        
        if(this->isStereo) {
            GUI_stereoscopic = new ofxUILabelToggle(115,posY+=20, 120, 16,parentElement->getDrawInStereo(),"Stereoscopic", OFX_UI_FONT_SMALL);
            UI->addWidget(GUI_stereoscopic);
        }
        
        if(parentElement->isWarpable)
        {
            GUI_QuadWarping = new ofxUIToggle(115, posY+=20, 16,16,false,"Quad Warping"); 
            UI->addWidget(GUI_QuadWarping);
            GUI_GridWarping = new ofxUIToggle(115, posY+=20, 16,16,false,"Fine Warping");
            UI->addWidget(GUI_GridWarping);
            
            GUI_xGridDecrease = new ofxUIImageButton(135, posY+=20, 16, 16, true, "./GUI/images/decrease.png", "xGridDecrease");
            UI->addWidget(GUI_xGridDecrease);
            GUI_xGridIncrease = new ofxUIImageButton(156, posY, 16, 16, true, "./GUI/images/increase.png", "xGridIncrease");
            UI->addWidget(GUI_xGridIncrease);
            UI->addWidget(new ofxUILabel(178, posY+4, "xGrid", "xGrid", OFX_UI_FONT_SMALL));
            
            GUI_yGridDecrease = new ofxUIImageButton(135, posY+=20, 16, 16, true, "./GUI/images/decrease.png", "yGridDecrease");
            UI->addWidget(GUI_yGridDecrease);
            GUI_yGridIncrease = new ofxUIImageButton(156, posY, 16, 16, true, "./GUI/images/increase.png", "yGridIncrease");
            UI->addWidget(GUI_yGridIncrease);
            UI->addWidget(new ofxUILabel(178, posY+4, "yGrid", "yGrid", OFX_UI_FONT_SMALL));
            
            GUI_resetWarp = new ofxUILabelButton(115, posY+=20, 55, false, "reset warp", OFX_UI_FONT_SMALL);
            UI->addWidget(GUI_resetWarp);
            GUI_resetGrid = new ofxUILabelButton(175, posY, 55, false, "reset grid", OFX_UI_FONT_SMALL);
            UI->addWidget(GUI_resetGrid);
        }
        
        
        posY=10;
        
        //seconda colonna: opacità e componenti r,g,b
        GUI_opacity = new ofxUIMinimalSlider(260, posY, 100,10,0.0,1.0,parentElement->getOpacity() ,"Opacity");
        UI->addWidget(GUI_opacity);
        GUI_red = new ofxUIMinimalSlider(260, posY+=20, 100,10, 0, 255, parentElement->getRed() ,"RED");
        UI->addWidget(GUI_red);
        GUI_green = new ofxUIMinimalSlider(260, posY+=20, 100,10, 0, 255, parentElement->getGreen() ,"GREEN");
        UI->addWidget(GUI_green);
        GUI_blue = new ofxUIMinimalSlider(260, posY+=20, 100,10, 0, 255, parentElement->getBlue() ,"BLUE");
        UI->addWidget(GUI_blue);
        GUI_resetRGB = new ofxUIButton(260, posY+=20, 16, 16, false, "RGB reset");
        UI->addWidget(GUI_resetRGB);
        
        GUI_orizMirror = new ofxUILabelButton(260, posY+=20, 55, false, "flipH", OFX_UI_FONT_SMALL);
        UI->addWidget(GUI_orizMirror);
        GUI_vertMirror = new ofxUILabelButton(320, posY, 55, false, "flipV", OFX_UI_FONT_SMALL);
        UI->addWidget(GUI_vertMirror);

        posY=10;
        
        //terza colonna: blending mode
        listBlendModes = new ofxUIDropDownList(390, posY, 100, "Blending Mode", blendingNames, OFX_UI_FONT_SMALL);
        listBlendModes->setDrawBack(true);
        listBlendModes->setDrawOutlineHighLight(false);
        listBlendModes->setDrawPaddingOutline(false);
        listBlendModes->setPadding(0);
        listBlendModes->setDrawFill(true);
        listBlendModes->setAutoClose(true);
        UI->addWidget(listBlendModes);
        
        //quarta colonna : input type (mono, two channel, left/right, top/bottom)
        GUI_inputType = new ofxUIDropDownList(500, posY, 100, "input", inputTypeNames, OFX_UI_FONT_SMALL);
        GUI_inputType->setDrawBack(true);
        GUI_inputType->setDrawOutlineHighLight(false);
        GUI_inputType->setDrawPaddingOutline(false);
        GUI_inputType->setPadding(0);
        GUI_inputType->setDrawFill(true);
        GUI_inputType->setAutoClose(true);
        UI->addWidget(GUI_inputType);

        
        
    }
    
    //but mixer needs output mode selection and many other things :) 
	else if (type==ELEMENT_MIXER)
    {
        
        UI = new ofxUICanvas(xPos,yPos+450, 600, 240);
        
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
        
        // ELEMENT MAIN CONTROL
        ofColor(0);
        UI->addWidget(new ofxUILabel(5, 10, "ELEMENT MAIN CONTROL", 1));
        
        ofxUISpacer* spacer = new ofxUISpacer(5, 30, 590, 1);
        spacer->setColorFill(ofColor(0, 255, 206));
        UI->addWidget(spacer);
        
        // STEREOSCOPIC PLAYER
        UI->addWidget(new ofxUILabel(5, 40, "STEREOSCOPIC PLAYER",2));
        
        // STEREOSCOPIC SETTING
        UI->addWidget(new ofxUILabel(300, 40, "STEREOSCOPIC SETTING",2));

        
        // VIEW GRID
        GUI_viewGridPreview = new ofxUILabelToggle(430, 6, 50, 20, true, "GRID", OFX_UI_FONT_SMALL);
        UI->addWidget(GUI_viewGridPreview);
        
        // WIDESCREEN PREVIEW
        GUI_widescreenPreview = new ofxUILabelToggle(490, 6, 100, 20, false, "PREVIEW 16:9", OFX_UI_FONT_SMALL);
        UI->addWidget(GUI_widescreenPreview);
                
        //MAIN OPACITY
        GUI_mainOpacity = new ofxUIMinimalSlider(490, 40, 100,20,0.0,1.0,parentElement->getOpacity() ,"MAIN OPACITY");
        UI->addWidget(GUI_mainOpacity);

        // STEREOSCOPIC
        GUI_mainStereo = new ofxUIToggle(300, 60, 16, 16, parentElement->getDrawInStereo(), "MAIN STEREO");
        UI->addWidget(GUI_mainStereo);
        
        // SWAP LEFT RIGHT
        GUI_swapLeftRight = new ofxUIToggle(300, 80, 16, 16, parentElement->getSwapLeftRight(), "SWAP LEFT RIGHT");
        UI->addWidget(GUI_swapLeftRight);

        // OUTPUT MODE
        listOutputModes = new ofxUIDropDownList(300, 100, "Output Mode", outputModesNames, OFX_UI_FONT_SMALL);
        listOutputModes->setDrawBack(true);
        listOutputModes->setDrawOutlineHighLight(false);
        listOutputModes->setDrawPaddingOutline(false);
        listOutputModes->setPadding( 0 );
        listOutputModes->setDrawFill(true);
        listOutputModes->setAutoClose(true);
        UI->addWidget(listOutputModes);
        
        // TEST PATTERN
        GUI_testPattern = new ofxUILabelToggle(490,70,100,20,false,"TEST PATTERN", OFX_UI_FONT_SMALL);
        UI->addWidget(GUI_testPattern);
        
        // FULL SCREEN
        GUI_fullscreen = new ofxUILabelToggle(490, 95, 100,20,false, "FULL SCREEN", OFX_UI_FONT_SMALL);
        UI->addWidget(GUI_fullscreen);
        
        // SAVE PROJECT
        UI->addWidget(new ofxUILabelButton(490, 180, 100, false, "SAVE PROJECT", OFX_UI_FONT_SMALL));
        
        // OPEN PROJECT
        UI->addWidget(new ofxUILabelButton(490, 150, 100, false, "OPEN PROJECT", OFX_UI_FONT_SMALL));

        
        
        // ALTERNATIVE PLAYER
        posY=60;
        GUI_videoNavigator = new ofxUIMinimalSlider(marginLeft, posY, 210,16,0.0,1.0, 0 ,"VIDEO NAVIGATOR");
        UI->addWidget(GUI_videoNavigator);        
        
        posY=80;
        UI->addWidget(new ofxUILabelButton(marginLeft, posY, 60,false,"REW 2x", OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUILabelButton(marginLeft+65, posY, 80,false,"PLAY", OFX_UI_FONT_LARGE));
        UI->addWidget(new ofxUILabelButton(marginLeft+150, posY, 60,false,"FFW 2x", OFX_UI_FONT_SMALL));
        posY=110;    
        UI->addWidget(new ofxUILabelButton(marginLeft, posY, 60,false,"PREV", OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUILabelButton(marginLeft+150, posY, 60,false,"NEXT", OFX_UI_FONT_SMALL));
        
        posY=140;
        UI->addWidget(new ofxUILabelButton(marginLeft, posY, 60,false, "PAUSE", OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUILabelButton(marginLeft+65, posY-22, 80,false, "STOP", OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUILabelToggle(marginLeft+150, posY, 60,false,"LOOP", OFX_UI_FONT_SMALL));
        UI->addWidget(new ofxUILabelToggle(marginLeft+65, posY+8, 80,16,false, "MUTE", OFX_UI_FONT_SMALL));

        posY=170;
        GUI_volume = new ofxUIMinimalSlider(marginLeft, posY, 210,16,0.0,1.0, 1 ,"SOUND VOLUME");
        UI->addWidget(GUI_volume);

        
        posY=196;
        UI->addWidget(new ofxUILabel(marginLeft, posY, "FRAME LEFT", 2));
        UI->addWidget(new ofxUILabel(marginLeft, posY+= 12, "FRAME RIGHT", 2));

              
    }

    if (type!=ELEMENT_MIXER) ofAddListener(UI->newGUIEvent,this,&elementUIBase::guiEvent); 
    

}

//--------------------------------------------------------------
void elementUIBase::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	
    //INPUT MODE
    if( e.widget->getParent()->getName()=="input")
	{
        if(name=="MONO") parentElement->setElementInputType(ELM_INPUT_MONO);
        else if(name=="2CHANNEL") parentElement->setElementInputType(ELM_INPUT_STEREO_TWO_CHANNEL);
        else if(name=="LEFTRIGHT")parentElement->setElementInputType(ELM_INPUT_STEREO_LEFTRIGHT);
        else if(name=="TOPBOTTOM")parentElement->setElementInputType(ELM_INPUT_STEREO_TOPBOTTOM);
        
        parentElement->isSelected=true;
		parentElement->warper.bViewGrid=true;
        parentElement->warper.decreaseXgrid();
        parentElement->warper.increaseXgrid();
        parentElement->warper.bViewGrid=false;
        parentElement->isSelected=false;
        
	}    
    //ELEMENT OPACITY
	if(e.widget->getName()=="Opacity")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setOpacity(slider->getScaledValue());
	}
    //RED CHANNEL
	else if(e.widget->getName()=="RED")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setRed(int(slider->getScaledValue()));
    }
    //GREEN CHANNEL
    else if(e.widget->getName()=="GREEN")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setGreen(int(slider->getScaledValue()));
    }
    //BLUE CHANNEL
    else if(e.widget->getName()=="BLUE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setBlue(int(slider->getScaledValue()));
    }
    //RESET COLOR CHANNEL
    else if(e.widget->getName()=="RGB reset")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {
        parentElement->setRed(255);
        parentElement->setGreen(255);
        parentElement->setBlue(255);
        cout << "RGB RESET" << cout;
        }
    }
    
    //HORIZONTAL MIRROR
    else if(e.widget->getName()=="flipH")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {
        parentElement->warper.mirror(true, false);
        }

    }
    //VERTICAL MIRROR
    else if(e.widget->getName()=="flipV")
	{
		ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {
        parentElement->warper.mirror(false, true);
        }
    }

    
    
    //IS ACTIVE
    else if(e.widget->getName()=="isActive")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setIsActive(toggle->getValue());

		if (toggle->getValue()) parentElement->setOpacity(1);
        else parentElement->setOpacity(0);
        
	}
    //QUAD WARPING
    else if (e.widget->getName()=="Quad Warping")
    {
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if(parentElement->getIsActive()) {
            
            for (int i=1; i<4; i++) 
            {
                allElementsPointer[i]->isSelected=true;
                allElementsPointer[i]->warper.bWarpActive=false;
                allElementsPointer[i]->warper.bViewGrid=false;
                allElementsPointer[i]->isSelected=false;
            }
            
                parentElement->isSelected=true;
                parentElement->warper.bWarpActive=toggle->getValue();
                parentElement->isSelected=toggle->getValue();
        }
    }
    //FINE WARPING
    else if (e.widget->getName()=="Fine Warping")
    {
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if(parentElement->getIsActive()) {
            
            for (int i=1; i<4; i++) 
            {
                allElementsPointer[i]->isSelected=true;
                allElementsPointer[i]->warper.bWarpActive=false;
                allElementsPointer[i]->warper.bViewGrid=false;
                allElementsPointer[i]->isSelected=false;
            }

            parentElement->isSelected=true;
            parentElement->warper.bViewGrid=toggle->getValue();
            parentElement->isSelected=toggle->getValue();

        }
    }
    //X GRID DECREASE
    else if (e.widget->getName()=="xGridDecrease")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {

		if(parentElement->getIsActive() && parentElement->isSelected && parentElement->warper.bViewGrid)
        {
            cout << "X grid decrease" << endl;
            parentElement->warper.decreaseXgrid();
        }        
        }
    }
    //X GRID INCREASE
    else if (e.widget->getName()=="xGridIncrease")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {

		if(parentElement->getIsActive() && parentElement->isSelected && parentElement->warper.bViewGrid)
        {
            parentElement->warper.increaseXgrid();
        }
        }
    }
    //Y GRID DECREASE
    else if (e.widget->getName()=="yGridDecrease")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {

		if(parentElement->getIsActive() && parentElement->isSelected && parentElement->warper.bViewGrid)
        {
            parentElement->warper.decreaseYgrid();
        }
        }
    }
    //Y GRID INCREASE
    else if (e.widget->getName()=="yGridIncrease")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {

		if(parentElement->getIsActive() && parentElement->isSelected && parentElement->warper.bViewGrid)
        {
            parentElement->warper.increaseYgrid();
        }
        }
    }
    //RESET WARP
    else if (e.widget->getName()=="reset warp")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {

		if(parentElement->getIsActive() && parentElement->isSelected)
        {
            parentElement->warper.resetCorners();
        }
        }
    }
    //RESET GRID
    else if (e.widget->getName()=="reset grid")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        if (button->getValue()) {

		if(parentElement->getIsActive() && parentElement->isSelected)
        {
            parentElement->warper.decreaseXgrid();
            parentElement->warper.increaseXgrid();

        }
        }
    }
    //IS DRAW IN STEREO
    else if(e.widget->getName()=="Stereoscopic")
        {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        parentElement->setDrawInStereo(toggle->getValue());
        }
    //BLENDING MODE
    else if( e.widget->getParent()->getName()=="Blending Mode")
	{
            if(name=="Disabled") parentElement->setBlendingMode(OF_BLENDMODE_DISABLED);
            else if(name=="Alpha") parentElement->setBlendingMode(OF_BLENDMODE_ALPHA);
            else if(name=="Add")parentElement->setBlendingMode(OF_BLENDMODE_ADD);
            else if(name=="Subtract")parentElement->setBlendingMode(OF_BLENDMODE_SUBTRACT);
            else if(name=="Multiply")parentElement->setBlendingMode(OF_BLENDMODE_MULTIPLY);
            else if(name=="Screen")parentElement->setBlendingMode(OF_BLENDMODE_SCREEN);
        
	}
    //FULL SCREEN (seconda finestra)
    if(e.widget->getName()=="FULL SCREEN")
    {
        ofxFensterManager::get()->getWindowById(1)->toggleFullscreen();
        
        if (ofxFensterManager::get()->getWindowById(1)->getWindowMode() == OF_WINDOW)
        {
            ofxFensterManager::get()->getWindowById(1)->setWindowShape(400, 300);
            ofxFensterManager::get()->getWindowById(1)->setWindowPosition(400, 200);
        }
    }

//    for (int i=1; i<4; i++) 
//    {
//        allElementsPointer[i]->elementUIBase::aggiornaGUI();
//    };
}


//--------------------------------------------------------------
void elementUIBase::aggiornaGUI()
{
    if (type!=ELEMENT_MIXER)
    {
        GUI_isActive->setValue(parentElement->getIsActive());
//        GUI_stereoscopic->setValue(parentElement->getDrawInStereo());
        GUI_QuadWarping->setValue(parentElement->warper.bWarpActive);
        GUI_GridWarping->setValue(parentElement->warper.bViewGrid);
        GUI_opacity->setValue(parentElement->getOpacity());
        GUI_red->setValue(parentElement->getRed());
        GUI_green->setValue(parentElement->getGreen());
        GUI_blue->setValue(parentElement->getBlue());
    }
}
