#ifndef ELEMENTVIDEO
#define ELEMENTVIDEO

#include "ofMain.h"
#include "element.h"

// we considered NOT to use ofxQtKitVideoPlayer as James George said :
// QTKit stuff is fast, but at the expense of being asynchronous. You'll have to really hack it to get two videos to play.
// IMO you could load both side-by-side into one video, and then find a way to flip-flip between the two frames each frame, 
// half the time show the left half of the video, half the time show the right -- that's what i've always done with stereo projections and HD videos.

class elementVideo : public element
{
	public :
	
	elementVideo();
	
	void setup(string _leftChannel, string _rightChannel, int _width, int _height, int inputType, int _posX, int _posY,string _name, bool _isWarpable);
	void drawIntoFbo(bool _drawMonoOrStereo){};
	void update();
	void drawLeft(int x, int y, int w, int h);
	void drawRight(int x, int y, int w, int h);

    void setElementInputType(int _inType);
    void openFile(string _path){};
    
    //textures
	ofTexture& getLeftTexture();
	ofTexture& getRightTexture();
    
    //stereo input utilities:
    string  leftChannelPath;
    string  rightChannelPath;
    ofTexture lT;
    ofTexture rT;
    ofFbo tempLeft;
    ofFbo tempRight;

    //funzioni del player:
    void element_videoPlay(int _speed);
    void element_videoPause();
    void element_videoStop();
    void element_frameAvanti();
    void element_frameIndietro();
    void element_goToFrame(int _frame);
    void element_setLoop(ofLoopType _loop);
    void element_toggleSound();
    void element_soundVolume(float _volume);
    
    bool    mute;
    float   volume;
    float   lastVolume;
    
    //video player
	ofVideoPlayer leftChannelPlayer;
	ofVideoPlayer rightChannelPlayer;

};

#endif