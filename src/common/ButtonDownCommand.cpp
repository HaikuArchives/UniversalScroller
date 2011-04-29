/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "button.h"
#include "ButtonDownCommand.h"

void ButtonDownCommand::check_for_click( const char *cmd, const char *click, int index, int count )
{
	if (strncasecmp( cmd, click, strlen( click ) )==0 )
	{
		mouseButtonIndex = index;

		mouseButtonClicks = count;

		if ( strlen( cmd ) != strlen( click ) )
		{
			mouseButtonClicks *= atoi( cmd + strlen( click ) );
		}
	}
}
	
ButtonDownCommand::ButtonDownCommand(const char *cmd)
{
	kind = CMD_KIND_UNKNOWN_KIND;
	mouseButtonIndex = CMD_MOUSE_BUTTON_INDEX_UNKNOWN;
	mouseButtonClicks = 0;
	command=cmd;
	
	check_for_click( cmd, LEFT  , 0, 1 );
	check_for_click( cmd, RIGHT , 1, 1 );
	check_for_click( cmd, MIDDLE, 2, 1 );

	check_for_click( cmd, LEFTDBL  , 0, 2 );
	check_for_click( cmd, RIGHTDBL , 1, 2 );
	check_for_click( cmd, MIDDLEDBL, 2, 2 );
	
}

ButtonDownCommand::ButtonDownCommand( )
{
	kind = CMD_KIND_UNKNOWN_KIND;
	mouseButtonIndex = CMD_MOUSE_BUTTON_INDEX_UNKNOWN;
	mouseButtonClicks = 0;
	command=NULL;
}
