#include "elementUIBase.h"

// we can include element.h to have it's reference to methods and variables
// because we added a forwarded declaration on the .h
// in james words :
#include "element.h"

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
	
	
}
//--------------------------------------------------------------
void elementUIBase::setupUI(element* _parentElement)
{
	printf("setupUI %d %d\n",xPos,yPos);
	parentElement =  _parentElement;
	
    UI = new ofxUICanvas(xPos,yPos, 200,800);

	
	listBlendModes = new ofxUIDropDownList(100, "Blending Mode", blendingNames, OFX_UI_FONT_SMALL);
	listBlendModes->setAutoClose(true);
	
    UI->addWidgetDown(new ofxUILabel(parentElement->getElementName(), OFX_UI_FONT_MEDIUM));
	UI->addWidgetDown(new ofxUIToggle(20,20,parentElement->getIsActive(),"isActive"));
    UI->addWidgetDown(new ofxUISlider(100,20,0.0,1.0,parentElement->getOpacity() ,"opacity"));
	UI->addWidgetDown(listBlendModes);
	ofAddListener(UI->newGUIEvent,this,&elementUIBase::guiEvent);    

}

//--------------------------------------------------------------
void elementUIBase::guiEvent(ofxUIEventArgs &e)
{	
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 
	
	if(e.widget->getName()=="opacity")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		parentElement->setOpacity(slider->getScaledValue());
	}
	else if(e.widget->getName()=="isActive")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		parentElement->setIsActive(toggle->getValue());
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
}

////--------------------------------------------------------------
//void elementUIBase::setGUIOrigin(int _x,int _y)
//{
//	xPos = _x;
//	yPos = _y;
//	
//}


