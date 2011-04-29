/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
#ifndef _H_BUTTON_DOWN_COMMAND
#define _H_BUTTON_DOWN_COMMAND

#include <SupportDefs.h>

#define CMD_KIND_UNKNOWN_KIND -1

#define CMD_MOUSE_BUTTON_INDEX_UNKNOWN -1


class ButtonDownCommand {
public:
	ButtonDownCommand();	
	ButtonDownCommand( const char * );	

	int kind;
	int mouseButtonIndex;
	int mouseButtonClicks;
	const char * command;
private:
	void check_for_click( const char *, const char *, int, int );

};

#endif // _H_BUTTON_DOWN_COMMAND
