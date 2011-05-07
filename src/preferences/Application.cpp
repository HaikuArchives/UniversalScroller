/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
 
#include "Application.h"
#include "MainWindow.h"

Application::Application()
		  		  : BApplication("application/x-vnd.TM.USPref")
{
	MainWindow		*mainWindow;
	mainWindow = new MainWindow( BRect( 50, 50, 420, 375 ) );

	mainWindow->Show();
	
}
