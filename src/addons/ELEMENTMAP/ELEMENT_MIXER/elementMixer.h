#ifndef ELEMENTMIXER
#define ELEMENTMIXER


// modalità di output:
#define ELM_STEREO_ANAGLYPH		0
#define ELM_MONO                1
#define ELM_STEREO_OPENGL		2

// forse queste vanno all'interno degli element, non nel mixer.....????
#define ELM_STEREO_LEFTRIGHT	3
#define ELM_STEREO_TOPBOTTOM	4

#include "ofMain.h"
#include "element.h"

class MainWindow;


class elementMixer : public element
{
	public :
	
    //costruttore:
	elementMixer(){};  
    
    //metodi:
	void update();
//	void setup(int _width, int _height, int _stereoMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name, bool _isWarpable);
    
    void setup(MainWindow* _mainWindow, int _width, int _height, int _outputMode,element** _elements,int _numOfElements,int* _elementsOrder,int _posX, int _posY,string _name, bool _isWarpable);

    
    void drawIntoFbo(bool _drawMonoOrStereo);
	void drawOutput(int _x, int _y,int _width, int _height);
	void drawLeft(int x, int y, int w, int h){};
	void drawRight(int x, int y, int w, int h){};
	
    void drawPreview(int x, int y, int w, int h);
    void drawInfo();
    
    void guiEvent(ofxUIEventArgs &e);
    
    //utilities:
    int  getOutputMode();
	void setOutputMode(int _mode);
	
    
    ofTexture& getLeftTexture(){};
	ofTexture& getRightTexture(){};

    MainWindow*  mainWindow;
        
    ofFbo        fboAnagliph;
	
    
    private :

	element**			sceneElements;
	int					numOfElements;
	int*				elementsOrder;

    int                 actualVolume;

    int					outputMode;
	
	
};

#endif