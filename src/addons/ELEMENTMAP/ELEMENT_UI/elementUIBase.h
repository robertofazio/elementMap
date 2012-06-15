
#ifndef ELEMENT_UI_BASE
#define ELEMENT_UI_BASE

#include "ofxUI.h"
//#include "element.h"


class element;

class  elementUIBase
{
    public :
    
    void setupUI(element* parentElement);
    //void setupUI(void* parentElement);
    void guiEvent(ofxUIEventArgs &e);   
	
	ofxUICanvas*		UI;
	element*			parentElement;
};
#endif