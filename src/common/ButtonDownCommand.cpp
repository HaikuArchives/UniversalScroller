/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */

#include <Message.h>
#include <Message.h>
#include <View.h>
#include <List.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_strings.h"
#include "ButtonDownCommand.h"

static char * cloneCommand( const char * str )
{
	char * clone=(char *) malloc( MAX_COMMAND_LENGTH );
	strncpy( clone, str, MAX_COMMAND_LENGTH );
 	clone[ MAX_COMMAND_LENGTH - 1 ]=0;
	return clone;
}

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
	
	command = cloneCommand( str );

	check_for_click( str, LEFT  , button, CMD_MOUSE_BUTTON_INDEX_PRIMARY  , 1 );
	check_for_click( str, RIGHT , button, CMD_MOUSE_BUTTON_INDEX_SECONDARY, 1 );
	check_for_click( str, MIDDLE, button, CMD_MOUSE_BUTTON_INDEX_TERTIARY , 1 );

	check_for_click( str, LEFTDBL  , button, CMD_MOUSE_BUTTON_INDEX_PRIMARY  , 2 );
	check_for_click( str, RIGHTDBL , button, CMD_MOUSE_BUTTON_INDEX_SECONDARY, 2 );
	check_for_click( str, MIDDLEDBL, button, CMD_MOUSE_BUTTON_INDEX_TERTIARY , 2 );
	
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

ButtonDownCommand::ButtonDownCommand(const ButtonDownCommand &cmd )
{
	kind = cmd.kind;
	mouseButtonIndex = cmd.mouseButtonIndex;
	mouseButtonClicks = cmd.mouseButtonClicks;
	command = cloneCommand( cmd.command );
}

ButtonDownCommand::~ButtonDownCommand( )
{
	free( (char *) command );
}

ButtonDownCommand & ButtonDownCommand::operator=(const ButtonDownCommand &rhs )
{
	if ( this != &rhs )
	{
		kind = rhs.kind;
	
		mouseButtonIndex = rhs.mouseButtonIndex;

		mouseButtonClicks = rhs.mouseButtonClicks;

		free( (char *) command );
		command = cloneCommand( rhs.command );
	}
	return *this;
}

int32 ButtonDownCommand::mouseButtonIndexToMask( int index )
{
	int32 mask = 0;
	switch ( index )
	{
		case CMD_MOUSE_BUTTON_INDEX_PRIMARY:
			mask = B_PRIMARY_MOUSE_BUTTON;
			break;
			
		case CMD_MOUSE_BUTTON_INDEX_SECONDARY:
			mask = B_SECONDARY_MOUSE_BUTTON;
			break;
		
		case CMD_MOUSE_BUTTON_INDEX_TERTIARY:
			mask = B_TERTIARY_MOUSE_BUTTON;
			break;
	
	}
	return mask;
}
