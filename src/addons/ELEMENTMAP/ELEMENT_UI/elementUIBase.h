
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

	
	ofxUICanvas*			UI;
	element*				parentElement;
    
    element**               allElementsPointer;
	
	//void setGUIOrigin(int _x,int _y);
	int						xPos;
	int						yPos;
	
	protected :
	
	ofxUIDropDownList*		listBlendModes;
	ofxUIDropDownList*		listOutputModes;
	ofxUIDropDownList*		listResolution;
	vector<string>			blendingNames; 
	vector<string>          outputModesNames;
    
    bool                    isStereo;
    
    


};
#endif