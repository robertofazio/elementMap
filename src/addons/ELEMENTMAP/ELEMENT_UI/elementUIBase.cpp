#include "elementUIBase.h"
#include "element.h"

void elementUIBase::setupUI(element* _parentElement)
{
	parentElement =  _parentElement;
	
    // this looks strange ... need to be tested 
    // if doesn't work then make ofxUICanvas a member of the class not derived 
    UI = new ofxUICanvas(20,20, 220, 220);
    UI->addWidgetDown(new ofxUILabel("GUI", OFX_UI_FONT_MEDIUM));
    UI->addWidgetDown(new ofxUISlider(100,20,0.0,1.0,parentElement->getOpacity() ,"opacity"));
						
	ofAddListener(UI->newGUIEvent,this,&elementUIBase::guiEvent);    

}

//--------------------------------------------------------------
void elementUIBase::guiEvent(ofxUIEventArgs &e)
{
	printf("We are on baseElementUIBase !!\n");
	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 
	
	if(e.widget->getName()=="opacity");
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setOpacity(slider->getScaledValue());
	}
}