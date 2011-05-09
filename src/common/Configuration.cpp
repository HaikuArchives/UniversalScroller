/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#include <SupportDefs.h>

#include "command_strings.h"

#include "Configuration.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <View.h>


Configuration::Configuration()
{
	load();
}

static const char *getConfigurationFileName( void )
{
	const char *env_line=getenv("HOME");
	char *ret;
	if ( env_line )
	{
		int len = strlen( env_line ) + 1 + 19;
		ret=(char *) malloc( len );
		snprintf( ret, len, "%s/.universalscroller", env_line );
		ret[len]=0;
	} else {
		ret=(char *) malloc( 30 );
		strncpy( ret, "/boot/home/.universalscroller", 30 );
	}
	return (const char *) ret;	
}

void Configuration::loadFallbackConfiguration( void )
{
	int i;
	
	minScroll=0;
	doubleClickSpeed[0]=250000;
	doubleClickSpeed[1]=250000;
	doubleClickSpeed[2]=250000;

	wheelFactorX[0]= 1.0;
	wheelFactorY[0]= 1.0;
	wheelFactorX[1]=10.0;
	wheelFactorY[1]=10.0;
	useWheelFactors=true;

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
		// The following condition is true, when at least two mouse buttons
		// are pressed
		scrollWhenMouseDown[i] = ( (i==3) || (i>4) );
	}

	// First we default to swallowing all clicks ...
	for ( i=0; i<9; i++ )
	{
		swallowClick[i] = true;
	}
	// ... then we reenable simple clicks
	swallowClick[getButtonDownIndex( 0, B_PRIMARY_MOUSE_BUTTON   )] = false;
	swallowClick[getButtonDownIndex( 0, B_SECONDARY_MOUSE_BUTTON )] = false;
	swallowClick[getButtonDownIndex( 0, B_TERTIARY_MOUSE_BUTTON  )] = false;

}

#define READ( POS, LEN ) \
	if ( cont ) \
	{ \
		read_result = read( handle, POS, LEN ); \
		cont = ( read_result == LEN ); \
	}

void Configuration::load( void )
{
	loadFallbackConfiguration();
	const char *filename = getConfigurationFileName();
	int handle=open( filename, O_RDONLY );
	
	if (handle >= 0)
	{
		int8 i8;
		int32 i32;
		int i;
		int read_result;
		bool cont = true;
		
		READ( &i32, 4 );
        if (cont)
        {
        	cont = 1;
        }
        				
		READ( &minScroll    , 1*4 );
		for (i=0;i<2;i++)
		{
			READ( &(doubleClickSpeed[i]), 4 );
		}
		for (i=0;i<9;i++)
		{
			char command[ MAX_COMMAND_LENGTH ];

			READ( command, MAX_COMMAND_LENGTH );

			buttonDownCommand[i] = ButtonDownCommand( command );
		}
		for (i=0;i<9;i++)
		{
			READ( &i8, 1 )
			swallowClick[i] = ( i8 != 0 );
		}
		
		READ( &i8, 1 )
		useWheelFactors = ( i8 != 0 );
		
		READ( &(wheelFactorX[0]), 4 );
		READ( &(wheelFactorY[0]), 4 );
		READ( &(wheelFactorX[1]), 4 );
		READ( &(wheelFactorY[1]), 4 );
		for (i=0;i<8;i++)
		{
			READ( &i8, 1 )
			scrollWhenMouseDown[i] = ( i8 != 0 );
		}

		close( handle );	
	}
	free( (char *) filename );
}

void Configuration::save( void )
{
	const char *filename = getConfigurationFileName();
	int handle=open( filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR|S_IWUSR );

	if (handle >= 0)
	{
		int8 i8;
		int32 i32;
		int i;
		
		i32=1; // Configuration Version
		write( handle, &i32, 4 );
		
		write( handle, &minScroll    , 4 );
		for (i=0;i<2;i++)
		{
			write( handle, &(doubleClickSpeed[i]) , 4 );
		}
		for (i=0;i<9;i++)
		{
			write( handle, buttonDownCommand[i].command, MAX_COMMAND_LENGTH );
		}
		for (i=0;i<9;i++)
		{
			i8=( swallowClick[i] ? 1 : 0 );
			write( handle, &i8, 1 );
		}

		i8=( useWheelFactors ? 1 : 0 );
		write( handle, &i8, 1 );

		write( handle, &(wheelFactorX[0]), 4 );
		write( handle, &(wheelFactorY[0]), 4 );
		write( handle, &(wheelFactorX[1]), 4 );
		write( handle, &(wheelFactorY[1]), 4 );

		for (i=0;i<8;i++)
		{
			i8=( scrollWhenMouseDown[i] ? 1 : 0 );
			write( handle, &i8, 1 );
		}
		
		close( handle );	
	}
	free( (char *) filename );
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
