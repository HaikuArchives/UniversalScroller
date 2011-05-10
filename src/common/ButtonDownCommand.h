/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * ButtonDownCommand.h - protopypes for commands and actions to occur on 
 *   (inter-)clicks
 *
 * -------------------------------------------------------------------------
 * 
 * Copyright (C) 2011  Christian Aistleitner <christian@quelltextlich.at>
 *
 * -------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * -------------------------------------------------------------------------
 */

#ifndef _H_BUTTON_DOWN_COMMAND
#define _H_BUTTON_DOWN_COMMAND

#include <SupportDefs.h>

enum ButtonDownCommandKind { unknown, button, key, cut, copy, paste, executable };

#define CMD_MOUSE_BUTTON_INDEX_UNKNOWN  -1
#define CMD_MOUSE_BUTTON_INDEX_PRIMARY   0
#define CMD_MOUSE_BUTTON_INDEX_SECONDARY 1
#define CMD_MOUSE_BUTTON_INDEX_TERTIARY  2
#define CMD_MOUSE_BUTTON_INDICES_COUNT   3

// The maximal length of ther command member
#define MAX_COMMAND_LENGTH 250

class ButtonDownCommand {
public:
	ButtonDownCommand();
	ButtonDownCommand( const ButtonDownCommand & );
	
	// The given string is copied, hence the caller is resposible for freeing it	
	ButtonDownCommand( const char * );	

	~ButtonDownCommand();

	ButtonDownCommand & operator=( const ButtonDownCommand & );
	
	ButtonDownCommandKind kind;
	int mouseButtonIndex;
	int mouseButtonClicks;
	const char * command;
	
	static int32 mouseButtonIndexToMask( int );
private:
	void check_for_click( const char *, const char *, ButtonDownCommandKind, int, int );
    void check_for_simple_command( const char *, const char *, ButtonDownCommandKind );

};

#endif // _H_BUTTON_DOWN_COMMAND
