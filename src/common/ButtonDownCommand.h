/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#ifndef _H_BUTTON_DOWN_COMMAND
#define _H_BUTTON_DOWN_COMMAND

#include <SupportDefs.h>

enum ButtonDownCommandKind { unknown, button, key, cut, copy, paste, executable };

#define CMD_MOUSE_BUTTON_INDEX_UNKNOWN -1


class ButtonDownCommand {
public:
	ButtonDownCommand();	
	ButtonDownCommand( const char * );	

	ButtonDownCommandKind kind;
	int mouseButtonIndex;
	int mouseButtonClicks;
	const char * command;
private:
	void check_for_click( const char *, const char *, ButtonDownCommandKind, int, int );
    void check_for_simple_command( const char *, const char *, ButtonDownCommandKind );

};

#endif // _H_BUTTON_DOWN_COMMAND
