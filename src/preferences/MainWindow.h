/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
 
#ifndef _H_MAIN_WINDOW
#define _H_MAIN_WINDOW

#include <Window.h>

#include "Configuration.h"

class MainWindow : public BWindow 
{
	public:
		MainWindow(BRect frame); 
		virtual void MessageReceived(BMessage* message);
		virtual	bool QuitRequested();

	private:
		Configuration configuration;
				
		void updateControlsFromConfiguration( void );

};

#endif // _H_APPLICATION
