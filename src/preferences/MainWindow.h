/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
 
#ifndef _H_MAIN_WINDOW
#define _H_MAIN_WINDOW

#include <Application.h>
#include <Window.h>
#include <Button.h>
#include <RadioButton.h>
#include <TextView.h>
#include <stdio.h>
#include <stdlib.h>
#include <StringView.h>
#include <TextView.h>
#include <TabView.h>
#include <PopUpMenu.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <InterfaceDefs.h>

#include <termios.h>
#include <string.h>
#include <ctype.h>
#include <TextControl.h>
#include <CheckBox.h>

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

				int LINES;

};

#endif // _H_APPLICATION
