/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
 
#include <be/support/TypeConstants.h>
#include <add-ons/input_server/InputServerFilter.h>

#include "UniversalScroller.h"

extern "C" _EXPORT BInputServerFilter* instantiate_input_filter();

BInputServerFilter* instantiate_input_filter()
{
	return (new UniversalScroller());
}
