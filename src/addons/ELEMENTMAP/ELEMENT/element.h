
#ifndef ELEMENT_H
#define ELEMENT_H

#include "ofMain.h"
#include "elementUIBase.h"
#include "ElementFX.h"
#include "ElementFXMask.h"

#include "elementWarp.h"

class element : public ofNode , public elementUIBase 
{
	public :
	
	element(){};
	
	void init(int _type,int _width, int _height,int internalformat,string _name,bool _isStereo, bool _isWarpable);
	virtual void update() =0;
	virtual void drawIntoFbo(bool _drawMonoOrStereo) =0;
	virtual void drawGraphic(int x, int y, int w, int h);
	virtual void drawLeft(int x, int y, int w, int h) =0;
	virtual void drawRight(int x, int y, int w, int h) =0;
    virtual ofTexture& getLeftTexture() =0;
	virtual ofTexture& getRightTexture() =0;
	
	void			setIsStereo(bool _b);
	void			setDrawInStereo(bool _b);
	void			setWidth(int _i);
	void			setHeight(int i_);
	void			setInternalFormat(int _i);
	void			setSwapLeftRight(bool b);
	void			setOpacity(float f);
	void			setIsShow(bool b);
	void			setIsActive(bool b);
	void			setBlendingMode(int _i);
    void            initFont();
	
	bool			getIsStereo();
	bool			getDrawInStereo();
	int				getWidth();
	int				getHeight();
	int				getInternalFormat();
	int				getElementType();	
	bool			getSwapLeftRight();
	bool			getIsShow();
	bool			getIsActive();
	float			getOpacity();
	string			getElementName();
	int				getBlendingMode();
    
    bool            isSelected;
    bool            isWarpable;
    
    ofTrueTypeFont          georgiaitalic10;
    ofTrueTypeFont          georgiaitalic8;
	
	ofFbo			fboLeft;
	ofFbo			fboRight;
    
    elementWarp     warper;
    
	ElementFXMask newEffect;                                            // Non capisco perchè, ma se dichiaro l'effetto all'interno del metodo, poi ho problemi
    vector<ElementFX*>       effects;                                   // Mauro;
    // virtual void                    drawPreview(int x, int y, int w, int h) = 0;    // Mauro
    virtual void                    addFX(int type) = 0;                            // Mauro
    virtual void                    applyFX() = 0;
    
	// set protected member to be just accesible from their derived classes (as private members are not accessible)
	
	protected :
    
	void			setOpacityColor();
    
	bool			isActive;	// indicates that we don't want this element to loose any time of cpu, so doesn't draw in preview or in draw
	bool			isShow;		// indicates we want to keep the element to preview but not draw it on the output
	bool			isClear;	// indicates if the fbo's have been cleared to black to avoid clearing on every draw
	int				elementWidth;
	int				elementHeight;
	int				blendingMode;
    
	
	private :
	
	int				type;
    //	bool			isStereo;           // I put in elementUIBase.h -> Mauro
	int				internalFormat; //GL_RGB,GL_RGBA ...
	bool			drawInStereo;
	bool			swapLeftRight;
	float			opacity;
	string			elementName;
	
	
};

#endif