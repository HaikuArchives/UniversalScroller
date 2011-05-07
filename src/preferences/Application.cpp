/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
 
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

#include "Application.h"
#include "MainWindow.h"

Application::Application()
		  		  : BApplication("application/x-vnd.TM.USPref")
{
	MainWindow		*aWindow;
	BRect			aRect;
	int left=50,top=50;
	aRect.Set(left, top, left+370, top+325);
	aWindow = new MainWindow(aRect);

	aWindow->Show();
}
