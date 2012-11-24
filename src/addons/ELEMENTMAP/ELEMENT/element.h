
#ifndef ELEMENT_H
#define ELEMENT_H

#define ELM_INPUT_MONO                  0
#define ELM_INPUT_STEREO_TWO_CHANNEL    1
#define ELM_INPUT_STEREO_LEFTRIGHT      2
#define ELM_INPUT_STEREO_TOPBOTTOM      3

#include "ofMain.h"
#include "elementUIBase.h"

#include "ElementFX.h"
#include "ElementFXMask.h"

#include "elementWarp.h"

class element : public ofNode , public elementUIBase 
{
	public :
	
	element(){};

    void init(int _type,int _width, int _height,int internalformat,string _name,int _inputType, bool _isWarpable);
	virtual void update() =0;
	virtual void drawIntoFbo(bool _drawMonoOrStereo) =0;
	virtual void drawGraphic(int x, int y, int w, int h);
	virtual void drawLeft(int x, int y, int w, int h) =0;
	virtual void drawRight(int x, int y, int w, int h) =0;
    virtual ofTexture& getLeftTexture() =0;
	virtual ofTexture& getRightTexture() =0;
    	
    virtual void openLeft(string _path) =0;
    virtual void openRight(string _path) =0;
                          
	void			setIsStereo(bool _b);
	void			setDrawInStereo(bool _b);
	void			setWidth(int _i);
	void			setHeight(int i_);
	void			setInternalFormat(int _i);
    virtual void    setElementInputType(int _inType) =0;
	void			setSwapLeftRight(bool b);
	void			setOpacity(float f);
	void			setRed(int red);
	void			setGreen(int green);
	void			setBlue(int blue);
	void			setIsShow(bool b);
	void			setIsActive(bool b);
	void			setBlendingMode(ofBlendMode _bm);
    void            initFont();
    
	bool			getIsStereo();
	bool			getDrawInStereo();
	int				getWidth();
	int				getHeight();
	int				getInternalFormat();
	int				getElementType();	
    int             getElementInputType();
	bool			getSwapLeftRight();
	bool			getIsShow();
	bool			getIsActive();
	float			getOpacity();
    int             getRed();
    int             getGreen();
    int             getBlue();
	string			getElementName();
	ofBlendMode		getBlendingMode();
        
    bool            isSelected;
    bool            isWarpable;
    
    ofTrueTypeFont  fontMedium;
    
	ofFbo			fboLeft;
	ofFbo			fboRight;
    
    int             inputType; //MONO, TWO CHANNEL STEREO, LEFT/RIGHT, TOP/BOTTOM
    
    elementWarp     warper;
    
    //SAVE-LOAD SETTINGS
    void            saveSettings();
    void            loadSettings();    
    ofxXmlSettings	XML;
    
    string			elementName;
    
    //variabili per il mixer:
    bool            wideScreenPreview;
    bool            showGrid;
    int             parallax;
    
    //dimensioni dichiarate:
	int				elementWidth;
	int				elementHeight;
    
	// set protected member to be just accesible from their derived classes (as private members are not accessible)
	
	protected :
    
	void			setOpacityColor();
	bool			isActive;	// indicates that we don't want this element to loose any time of cpu, so doesn't draw in preview or in draw
	bool			isShow;		// indicates we want to keep the element to preview but not draw it on the output
	bool			isClear;	// indicates if the fbo's have been cleared to black to avoid clearing on every draw
	ofBlendMode		blendingMode;
    
    ofImage         noMedia;
	
	private :
	
	int				type;
	int				internalFormat; //GL_RGB,GL_RGBA ...
	bool			drawInStereo;
    int             r;
    int             g;
    int             b;
	float			opacity;
    bool			swapLeftRight;
	
	
};

#endif