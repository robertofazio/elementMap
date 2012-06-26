//
//  ElementFX.cpp
//  elementPlayer_a0.1_Eloi
//
//  Created by mauro on 23/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "ElementFX.h"
#include "element.h"

void ElementFX::init(int _type, ofTexture& originalLeftTexture)
{
    this->type = _type;
    this->originalLeftTexture = originalLeftTexture;
    this->width = this->originalLeftTexture.getWidth();
    this->height = this->originalLeftTexture.getHeight();
    this->isStereo = false;
   // this->isStereo = this->_element->getIsStereo();
    this->isActive = true;
    this->guiVisibile = true;
    this->previewWidth = 0;
    this->previewHeight = 0;
    this->resetFX();
    this->activeEvents();
    cout << "INIT FX" << endl;
}

ofTexture& ElementFX::getLeftTexture()
{
    return originalLeftTexture;
}

bool ElementFX::hitTest(int x, int y)
{
    if((((int)x > this->previewPos.x) && ((int)x < this->previewPos.x + this->previewWidth) ) && (((int)y > this->previewPos.y) && ((int)y < this->previewPos.y + this->previewHeight)))
        return true;
    else
        return false;
}

void ElementFX::setIsStereo(bool b)
{
    this->isStereo = b;
}

void ElementFX::setIsActive(bool a)
{
    this->isActive = a;
}

void ElementFX::setGUIVisible(bool g)
{
    this->guiVisibile = g;
//    if(this->guiVisibile)
//        this->activeEvents();
//    else
//        this->deactiveEvents();
}

bool ElementFX::getIsStereo()
{
    return this->isStereo;
}

bool ElementFX::getIsActive()
{
    return this->isActive;
}

bool ElementFX::getGUIVisible()
{
    return this->guiVisibile;
}

int ElementFX::getWidth()
{
    return this->width;
}

int ElementFX::getHeight()
{
    return this->height;
}

int ElementFX::getElementFXType()
{
    return this->type;
}