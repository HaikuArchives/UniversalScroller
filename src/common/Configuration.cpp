/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#include "Configuration.h"

#include <SupportDefs.h>

#include "button.h"
#include <string.h>

Configuration::Configuration()
{
	int i;
	
	this->minScroll=0;
	this->doubleClickSpeed[0]=250000;
	this->doubleClickSpeed[1]=250000;
	this->doubleClickSpeed[2]=250000;

	this->factorX[0]=0.1;
	this->factorY[0]=0.1;
	this->factorX[1]=1.0;
	this->factorY[1]=1.0;
	factorsforwheel=true;

	strncpy( cmd[0], LEFT    , CMD_LEN );
	strncpy( cmd[1], "beep"  , CMD_LEN );
	strncpy( cmd[2], "beep"  , CMD_LEN );
	strncpy( cmd[3], RIGHT   , CMD_LEN );
	strncpy( cmd[4], "beep"  , CMD_LEN );
	strncpy( cmd[5], "beep"  , CMD_LEN );
	strncpy( cmd[6], MIDDLE  , CMD_LEN );
	strncpy( cmd[7], COPY    , CMD_LEN );
	strncpy( cmd[8], PASTE   , CMD_LEN );
	for ( i=0; i<9; i++ )
	{
		cmd[i][CMD_LEN]=0;
	}

	for ( i=0; i<8; i++ )
	{
		scrollmousedown[i] = (i>3);
	}

	for ( i=0; i<8; i++ )
	{
		swallowclick[i] = (i>3);
	}

}

