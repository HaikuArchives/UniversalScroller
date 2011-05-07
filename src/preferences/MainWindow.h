/*
 * Copyright 2010 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the GPLv2 license.
 */
 
#ifndef _H_MAIN_WINDOW
#define _H_MAIN_WINDOW

#include <Window.h>
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
		
		// Pointers to the relevant Controls
		BTextControl *wheelFactorX_ctrl[2];
		BTextControl *wheelFactorY_ctrl[2];
		BCheckBox *useWheelFactors_ctrl;
		BCheckBox *scrollWhenMouseDown_ctrl[8];
		BCheckBox *swallowClick_ctrl;
		BTextControl *command_ctrl;
		BTextControl *minScroll_ctrl;
		BCheckBox *restartInputServer_ctrl;	
		
		//private member functions
		
		void updateControlsFromConfiguration( void );

        void setCommandControl( int );
        
        void preserveSettings( void );
};

#endif // _H_APPLICATION
