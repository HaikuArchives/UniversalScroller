/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#ifndef _H_CONFIGURATION
#define _H_CONFIGURATION

#include <SupportDefs.h>

#include "ButtonDownCommand.h"

#define SCROLL_MOUSE_DOWN_MASK 7

class Configuration {
public:
	Configuration();	

	int minScroll;	
	int doubleClickSpeed[3];
	ButtonDownCommand buttonDownCommand[9];
	bool swallowclick[9];
		
	float factorX[2];
	float factorY[2];
	bool factorsforwheel;
	
	bool scrollmousedown[8];

    static int getButtonDownIndex( int32, int32 );
private:

};

#endif // _H_CONFIGURATION
