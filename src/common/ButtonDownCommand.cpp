/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "button.h"
#include "ButtonDownCommand.h"

void ButtonDownCommand::check_for_click( const char *str, const char *cmd, ButtonDownCommandKind cmd_kind, int index, int count )
{
	if (strncasecmp( str, cmd, strlen( cmd ) ) == 0 )
	{
		kind = cmd_kind;
		
		mouseButtonIndex = index;

		mouseButtonClicks = count;
		if ( strlen( str ) != strlen( cmd ) )
		{
			mouseButtonClicks *= atoi( str + strlen( cmd ) );
		}
	}
}

void ButtonDownCommand::check_for_simple_command( const char *str, const char *cmd, ButtonDownCommandKind cmd_kind )
{
	if ( strcasecmp(str, cmd ) == 0 )
	{
		kind = cmd_kind;
	}
}

	
ButtonDownCommand::ButtonDownCommand(const char *str)
{
	kind = unknown;
	mouseButtonIndex = CMD_MOUSE_BUTTON_INDEX_UNKNOWN;
	mouseButtonClicks = 0;
	command=str;
	
	check_for_click( str, LEFT  , button, 0, 1 );
	check_for_click( str, RIGHT , button, 1, 1 );
	check_for_click( str, MIDDLE, button, 2, 1 );

	check_for_click( str, LEFTDBL  , button, 0, 2 );
	check_for_click( str, RIGHTDBL , button, 1, 2 );
	check_for_click( str, MIDDLEDBL, button, 2, 2 );
	
	check_for_simple_command( str, CUT  , cut   );
	check_for_simple_command( str, COPY , copy  );
	check_for_simple_command( str, PASTE, paste );
	
	if (strncasecmp( str, KEY, strlen( KEY ) ) == 0 )
	{
		kind = key;
	}
			
	if ( kind == unknown )
	{
		kind = executable;
	}
	
}

ButtonDownCommand::ButtonDownCommand( )
{
	kind = unknown;
	mouseButtonIndex = CMD_MOUSE_BUTTON_INDEX_UNKNOWN;
	mouseButtonClicks = 0;
	command=NULL;
}
