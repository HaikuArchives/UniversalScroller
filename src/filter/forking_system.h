/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * forking_system.h - Wrapper for system call to fork off into separate thread
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

#ifndef _H_FORKING_SYSTEM
#define _H_FORKING_SYSTEM

#include "ButtonDownCommand.h"

int32 forking_system_threadproc( const char *command )
{
	system( command );

	return B_OK;
}

void forking_system( const char *command )
{
	char local_command[ MAX_COMMAND_LENGTH ];

	strncpy( local_command, command, MAX_COMMAND_LENGTH );
	local_command[ MAX_COMMAND_LENGTH - 1 ] = 0;
	
	resume_thread(
		spawn_thread(
			(thread_entry) forking_system_threadproc, 
			"ownthreadsystem", 
			B_LOW_PRIORITY, 
			(char *) local_command
		)
	);
}

#endif
