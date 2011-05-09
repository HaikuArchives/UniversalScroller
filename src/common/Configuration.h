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
	// create and reload the configuration
	Configuration();	

	void Configuration::loadFallbackConfiguration( void );
	void load( void );
    void save( void );

	int32 minScroll;	
	int32 doubleClickSpeed[3];
	ButtonDownCommand buttonDownCommand[9];
	bool swallowClick[9];
		
	float wheelFactorX[2];
	float wheelFactorY[2];
	bool useWheelFactors;
	
	bool scrollWhenMouseDown[8];

    static int getButtonDownIndex( int32, int32 );
private:

};

#endif // _H_CONFIGURATION
