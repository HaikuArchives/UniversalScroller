/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#include <SupportDefs.h>

#include "button.h"

#include "Configuration.h"

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

	i=0;
	buttonDownCommand[i++] = ButtonDownCommand( LEFT   );
	buttonDownCommand[i++] = ButtonDownCommand( "beep" );
	buttonDownCommand[i++] = ButtonDownCommand( "beep" );
	buttonDownCommand[i++] = ButtonDownCommand( RIGHT  );
	buttonDownCommand[i++] = ButtonDownCommand( "beep" );
	buttonDownCommand[i++] = ButtonDownCommand( "beep" );
	buttonDownCommand[i++] = ButtonDownCommand( MIDDLE );
	buttonDownCommand[i++] = ButtonDownCommand( COPY   );
	buttonDownCommand[i++] = ButtonDownCommand( PASTE  );
	
	for ( i=0; i<8; i++ )
	{
		scrollmousedown[i] = (i>3);
	}

	for ( i=0; i<8; i++ )
	{
		swallowclick[i] = (i>3);
	}

}

