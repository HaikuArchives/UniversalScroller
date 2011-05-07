/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#include <SupportDefs.h>

#include "command_strings.h"

#include "Configuration.h"

#include <string.h>

Configuration::Configuration()
{
	int i;
	
	this->minScroll=0;
	this->doubleClickSpeed[0]=250000;
	this->doubleClickSpeed[1]=250000;
	this->doubleClickSpeed[2]=250000;

	this->factorX[0]= 1.0;
	this->factorY[0]= 1.0;
	this->factorX[1]=10.0;
	this->factorY[1]=10.0;
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

	for ( i=0; i<9; i++ )
	{
		swallowclick[i] = (i>3);
	}

}

int Configuration::getButtonDownIndex( int32 previousButtons, int32 currentButtons )
{
	int idx=-1;
	if ((previousButtons==0) && (currentButtons==1)) idx=0;
	if ((previousButtons==1) && (currentButtons==3)) idx=1;
	if ((previousButtons==1) && (currentButtons==5)) idx=2;
	if ((previousButtons==0) && (currentButtons==2)) idx=3;
	if ((previousButtons==2) && (currentButtons==3)) idx=4;
	if ((previousButtons==2) && (currentButtons==6)) idx=5;
	if ((previousButtons==0) && (currentButtons==4)) idx=6;
	if ((previousButtons==4) && (currentButtons==5)) idx=7;
	if ((previousButtons==4) && (currentButtons==6)) idx=8;
	
	return idx;
}
