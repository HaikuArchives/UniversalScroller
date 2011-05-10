/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * MainWindow.h - prototypes of the main window of the settings program 
 *   for UniversalScroller
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
        void preserveCommandControl( int );
        
        void preserveSettings( void );
};

#endif // _H_APPLICATION
