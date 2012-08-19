#ifndef ELEMENT_MAIN
#define ELEMENT_MAIN

//                          0            1         2               3            4               5
enum typeOfElement {ELEMENT_MASK,ELEMENT_VIDEO,ELEMENT_IMG,ELEMENT_SYPHON,ELEMENT_GEOMETRY,ELEMENT_MIXER};
//#define ELEMENT_MASK 0
//#define ELEMENT_VIDEO 1
//#define ELEMENT_IMG 2
//#define ELEMENT_SYPHON 3
//#define ELEMENT_GEOMETRY 4
//#define ELEMENT_MIXER 5

#include "element.h"
#include "elementVideo.h"
#include "elementSyphon.h"
#include "elementMixer.h"
#include "elementImage.h"

#endif