#include "ofxSimpleGuiMovieSlider.h"

//--------------------------------------------------------------------- movie slider
ofxSimpleGuiMovieSlider::	ofxSimpleGuiMovieSlider(string name, ofVideoPlayer& input) : ofxSimpleGuiControl(name) 
{

	barwidth  = 0;
	this->input	= &input;
	videoFrameRate = 30.0;
	videoLength = input.getDuration();
	videoTotalNumFrames = float(this->input->getTotalNumFrames());
	videoFrameRate = float(ceil(videoTotalNumFrames/videoLength));
	controlType = "MovieSlider";
	setSize(config->gridSize.x - config->padding.x, config->sliderHeight*3);
	setup();

}

//--------------------------------------------------------------------- update
void ofxSimpleGuiMovieSlider::setup() {
}

//--------------------------------------------------------------------- update
void ofxSimpleGuiMovieSlider::update() {
	
	if(!enabled) return;
	if(!input) return;
	if(!lock) barwidth = ofMap(input->getPosition(), 0.0, 1.0, 0.0, (float)width);
	videoFramePosition = input->getCurrentFrame();
	videoLength = this->input->getDuration();
	videoTotalNumFrames = float(this->input->getTotalNumFrames());
	videoFrameRate = float(ceil(videoTotalNumFrames/videoLength));
	
}

//--------------------------------------------------------------------- mouse dragged
void ofxSimpleGuiMovieSlider::onDragOver(int _x, int _y, int buton) {
	float pct = float(getMouseX()) - float(this->x);
	float temp = ofMap(pct, 0.0, width, 0.0, 1.0, true);
	input->setPosition(temp);
}

//--------------------------------------------------------------------- mouse pressed
void ofxSimpleGuiMovieSlider::onPress(int _x, int _y, int button) {
	float pct = float(getMouseX()) - float(this->x);
	float temp = ofMap(pct, 0.0, width, 0.0, 1.0, true);
	input->setPosition(temp);
}

//--------------------------------------------------------------------- mouse released
void ofxSimpleGuiMovieSlider::onRelease() {
}


//---------------------------------------------------------
void ofxSimpleGuiMovieSlider::draw(float x, float y) 
{

	//update postion of gui object
	setPos(x, y);
	//VALUE CLAMP
	barwidth = ofMap(float(videoFramePosition), 0.0, float(videoTotalNumFrames), 0.0, (float)width);
	if(barwidth > width) barwidth = width;
	else if(barwidth < 0) barwidth = 0;
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	
	ofEnableAlphaBlending();
	ofFill();
	
	setEmptyColor();
	ofRect(0, 0, width, config->sliderHeight);
	
	setFullColor();
	ofRect(0, 0, barwidth, config->sliderHeight);
	
	setTextBGColor();
	ofRect(0, config->sliderHeight, width, config->sliderTextHeight);
	
	setTextColor();
	
	int frames = int(videoFramePosition);
	div_t divresult;
	div_t divresult2;
	div_t divresult3;
	divresult = div (frames,videoFrameRate);
	divresult2 = div(divresult.quot,60);
	divresult3 = div(divresult2.quot,60);
	
	string s = ofToString(divresult3.quot)+ "h: " + ofToString(divresult2.quot%60) + "m :" + ofToString(divresult.quot%60) + "s :" + ofToString(divresult.rem) + "f";
	ofDrawBitmapString(s, 3, config->sliderHeight + 14);
	
	ofDisableAlphaBlending();
	glPopMatrix();
	

}
