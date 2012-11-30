
#ifndef ELEMENT_UI_BASE
#define ELEMENT_UI_BASE

#include "ofxUI.h"

// class is declared here as a forward declaration
// in james words :
// basically what that does is tell  elementUIBase 
// "I promise that when you compile there will be a class called element"
// but you don't get to see what it is until compilation
class element;
//class elementMixer;


class  elementUIBase
{
    public :
    
	elementUIBase();
    void setupUI(element* parentElement);
    //void setupUI(void* parentElement);
    void guiEvent(ofxUIEventArgs &e);   

    void aggiornaGUI();
	
	ofxUICanvas*			UI;
	element*				parentElement;
    
    element**               allElementsPointer;
	
	//void setGUIOrigin(int _x,int _y);
	int						xPos;
	int						yPos;
	
    //widgets per gli element:
    ofxUIDropDownList*      GUI_inputType;
    ofxUIToggle*            GUI_isActive;
    ofxUILabelToggle*       GUI_stereoscopic;
    ofxUIButton*            GUI_QuadWarping;
    ofxUIButton*            GUI_GridWarping; 
    ofxUIImageButton*       GUI_xGridDecrease;
    ofxUIImageButton*       GUI_xGridIncrease;    
    ofxUIImageButton*       GUI_yGridDecrease;
    ofxUIImageButton*       GUI_yGridIncrease;  
    ofxUILabelButton*       GUI_resetWarp;
    ofxUILabelButton*       GUI_resetGrid;
    ofxUIMinimalSlider*     GUI_opacity;
    ofxUIMinimalSlider*     GUI_red;
    ofxUIMinimalSlider*     GUI_green;
    ofxUIMinimalSlider*     GUI_blue;
    ofxUIButton*            GUI_resetRGB;
	ofxUIDropDownList*		listBlendModes;
    ofxUILabelButton*       GUI_orizMirror;
    ofxUILabelButton*       GUI_vertMirror;
    ofxUILabelButton*       GUI_rotCW;
    ofxUILabelButton*       GUI_rotCCW;
    ofxUIDropDownList*      GUI_openLeftFile;
    ofxUIDropDownList*      GUI_openRightFile;
    
    //widgets del mixer:
    ofxUILabelToggle*       GUI_viewGridPreview;
    ofxUILabelToggle*       GUI_widescreenPreview;
    ofxUIMinimalSlider*     GUI_mainOpacity;    
    ofxUIToggle*            GUI_mainStereo;
    ofxUIToggle*            GUI_swapLeftRight;
    ofxUIDropDownList*		listOutputModes;
    ofxUILabelToggle*       GUI_testPattern;
    ofxUILabelToggle*       GUI_fullscreen;
    ofxUILabelButton*       GUI_newOutputWindow;
    ofxUIMinimalSlider*     GUI_parallax;
    
    //quelli del player sono quasi tutti superflui, almeno per adesso...
    ofxUIMinimalSlider*     GUI_videoNavigator;
    ofxUIMinimalSlider*     GUI_volume;
    
	protected :
	
	vector<string>			blendingNames; 
	vector<string>          outputModesNames;
    vector<string>          inputTypeNames;
    
    int                     type;
    bool                    isStereo;
    
    
    


};
#endif