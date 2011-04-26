/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#ifndef _H_CONFIGURATION
#define _H_CONFIGURATION

#include <SupportDefs.h>

#define CMD_LEN 255

class Configuration {
public:
	Configuration();	

	int minScroll;	
	int doubleClickSpeed[3];
	char cmd[9][CMD_LEN+1];
	
	float factorX[2];
	float factorY[2];
	bool factorsforwheel;
	
	bool scrollmousedown[8];
	bool swallowclick[8];
private:

};

#endif // _H_CONFIGURATION
