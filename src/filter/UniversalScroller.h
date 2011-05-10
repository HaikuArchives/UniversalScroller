/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * UniversalScroller.h - prototypes for the filter itself
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

#ifndef _H_UniversalScroller
#define _H_UniversalScroller

#include <SupportDefs.h>
#include <add-ons/input_server/InputServerFilter.h>
#include <Message.h>
#include <List.h>

#include "Configuration.h"

class UniversalScroller : public BInputServerFilter 
{
	private:
		Configuration configuration;
	public:
		UniversalScroller();
		virtual ~UniversalScroller();
		virtual	filter_result Filter(BMessage *message, BList *outList);
    	virtual status_t InitCheck(); 
};

#endif // _H_UniversalScroller
