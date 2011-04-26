/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
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
