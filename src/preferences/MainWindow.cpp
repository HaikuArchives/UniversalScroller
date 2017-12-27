/*
 * UniversalScroller -- Enhancing input methods for BeOS and Haiku 
 *
 * -------------------------------------------------------------------------
 *
 * MainWindow.cpp - implementation of the main window and application logic
 *   of the settings program for UniversalScroller
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
  
#include <Application.h>
#include <LayoutBuilder.h>
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
#include <SeparatorView.h>
#include <TextControl.h>
#include <CheckBox.h>

#define QUIT_MSG 'BTQT'
#define MSG_CANCEL 'USCA'
#define MSG_OK 'USOK'
#define CHANGEEVENT_MSG 'USC'*0x100
#define CHANGEEVENTTYPE_MSG 'USET'
#define LEFT_MSG 'USLS'
#define LEFTDBL_MSG 'USLD'
#define MIDDLE_MSG 'USMS'
#define MIDDLEDBL_MSG 'USMD'
#define RIGHT_MSG 'USRS'
#define RIGHTDBL_MSG 'USRD'

#define CUT_MSG 'USCT'
#define COPY_MSG 'USCP'
#define PASTE_MSG 'USPT'

#include "command_strings.h"

#include "Configuration.h"
#include "MainWindow.h"


#define MAX_CONF_STR 250

MainWindow::MainWindow(BRect frame)
				: BWindow(frame, "UniversalScroller - Preferences", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE |B_AUTO_UPDATE_SIZE_LIMITS )
{
	BView *view;
	BTabView* tabView = new BTabView("tabview", B_WIDTH_FROM_LABEL);
	
	//
	// S C R O L L V I E W
	//
	view = new BView("Scrolling", B_WILL_DRAW);
    view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 

	scrollWhenMouseDown_ctrl[1]=new BCheckBox("PT1","left",NULL);
	scrollWhenMouseDown_ctrl[3]=new BCheckBox("PT3","left+right",NULL);
	scrollWhenMouseDown_ctrl[5]=new BCheckBox("PT5","left+middle",NULL);
	scrollWhenMouseDown_ctrl[7]=new BCheckBox("PT7","left+middle+right", NULL);
	scrollWhenMouseDown_ctrl[4]=new BCheckBox("PT4","middle",NULL);
	scrollWhenMouseDown_ctrl[6]=new BCheckBox("PT6","middle+right",NULL);
	scrollWhenMouseDown_ctrl[2]=new BCheckBox("PT2","right",NULL);

	minScroll_ctrl=new BTextControl(NULL, "","0",NULL);
	minScroll_ctrl->SetDivider(0);
	
	BLayoutBuilder::Group<>(view, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
		.Add(new BStringView( NULL, "Use UniversalScrolling for") )
		.Add(new BStringView( NULL, "those combinations of buttons:") )
		.AddGroup(B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
			.Add(scrollWhenMouseDown_ctrl[1] )
			.Add(scrollWhenMouseDown_ctrl[3] )
			.Add(scrollWhenMouseDown_ctrl[5] )
			.Add(scrollWhenMouseDown_ctrl[7] )
			.Add(scrollWhenMouseDown_ctrl[4] )
			.Add(scrollWhenMouseDown_ctrl[6] )
			.Add(scrollWhenMouseDown_ctrl[2] )
		.End()
		.Add(new BStringView(NULL, "Minimal mouse movement to"))
		.AddGroup(B_HORIZONTAL)
			.Add(new BStringView(NULL, "start UniversalScrolling:"))
			.Add(minScroll_ctrl)
		.End();

	tabView->AddTab(view);

	
	//
	// S P E E D V I E W
	//
	view=new BView("Scroll-Speed", B_WILL_DRAW);
    view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 

	useWheelFactors_ctrl=new BCheckBox(NULL, "Use below factors for scroll-wheels",NULL);

	wheelFactorX_ctrl[0]=new BTextControl(NULL, "Factor X","0",NULL);
	wheelFactorY_ctrl[0]=new BTextControl(NULL, "Factor Y","0",NULL);
	wheelFactorX_ctrl[1]=new BTextControl(NULL, "Factor X","0",NULL);
	wheelFactorY_ctrl[1]=new BTextControl(NULL, "Factor Y","0",NULL);

	
	BLayoutBuilder::Group<>(view, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
		.Add(useWheelFactors_ctrl)
		.AddGroup(B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
			.Add(new BStringView(NULL, "normal Scroll-Speed"))
			.AddGroup(B_VERTICAL, 0)
			.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
			.Add(wheelFactorX_ctrl[0])
			.Add(wheelFactorY_ctrl[0])
			.End()
		.End()
		.AddGroup(B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
			.Add(new BStringView(NULL, "fast Scroll-Speed"))
			.AddGroup(B_VERTICAL, 0)
			.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
			.Add(wheelFactorX_ctrl[1])
			.Add(wheelFactorY_ctrl[1])
			.End()
		.End();

	tabView->AddTab(view);
	//
	// C L I C K V I E W
	//
	view = new BView("Clicks", B_WILL_DRAW);
    view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 
	
	BMenu *menu;
	menu = new BMenu( "" );
	menu->SetLabelFromMarked( true );
	
	menu->AddItem(new BMenuItem("left down",new BMessage(CHANGEEVENT_MSG+0)));                           
	menu->AddItem(new BMenuItem("left down and right interclick",new BMessage(CHANGEEVENT_MSG+1)));
	menu->AddItem(new BMenuItem("left down and middle interclick",new BMessage(CHANGEEVENT_MSG+2)));
	menu->AddItem(new BMenuItem("right down",new BMessage(CHANGEEVENT_MSG+3)));
	menu->AddItem(new BMenuItem("right down and left interclick",new BMessage(CHANGEEVENT_MSG+4)));
	menu->AddItem(new BMenuItem("right down and middle interclick",new BMessage(CHANGEEVENT_MSG+5)));
	menu->AddItem(new BMenuItem("middle down",new BMessage(CHANGEEVENT_MSG+6)));
	menu->AddItem(new BMenuItem("middle down and left interclick",new BMessage(CHANGEEVENT_MSG+7)));
	menu->AddItem(new BMenuItem("middle down and right interclick",new BMessage(CHANGEEVENT_MSG+8)));	
	menu->ItemAt( 0 )->SetMarked( true );
	BMenuField *menuField;
	menuField = new BMenuField(NULL, NULL, menu );
	
	swallowClick_ctrl = new BCheckBox(NULL, "swallow this click", NULL);


	command_ctrl=new BTextControl(NULL, "", "", NULL);

	const int kMaxButton = 9;
	BButton* button[kMaxButton];
	BLayoutBuilder::Group<>(view, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING, B_USE_DEFAULT_SPACING)
		.Add(new BStringView(NULL,"What to do when ..."))
		.Add(menuField)
		.Add(swallowClick_ctrl)
		.Add(command_ctrl)
		.AddGrid(0.0, 0.0)
			.Add(button[0] = new BButton(NULL, "Left Sgl Click",new BMessage(LEFT_MSG)), 0, 0)
			.Add(button[1] = new BButton(NULL, "Middle Sgl Click",new BMessage(MIDDLE_MSG)), 1, 0)
			.Add(button[2] = new BButton(NULL, "Right Sgl Click",new BMessage(RIGHT_MSG)), 2,0)
			.Add(button[3] = new BButton(NULL, "Left Dbl Click",new BMessage(LEFTDBL_MSG)), 0, 1)
			.Add(button[4] = new BButton(NULL, "Middle Dbl Click",new BMessage(MIDDLEDBL_MSG)), 1, 1)
			.Add(button[5] = new BButton(NULL, "Right Dbl Click",new BMessage(RIGHTDBL_MSG)), 2, 1)
			.Add(button[6] = new BButton(NULL, "Cut",new BMessage(CUT_MSG)), 0, 2)
			.Add(button[7] = new BButton(NULL, "Copy",new BMessage(COPY_MSG)), 1, 2)
			.Add(button[8] = new BButton(NULL, "Paste",new BMessage(PASTE_MSG)), 2, 2);

	for (int i = 0; i < kMaxButton; i++)
			button[i]->SetExplicitMaxSize(BSize(B_SIZE_UNLIMITED, B_SIZE_UNSET));
	tabView->AddTab(view);


    // Button Panel at bottom ----------------------------------------------------------------

	restartInputServer_ctrl=new BCheckBox(NULL, "restart inputserver on OK",NULL);
    restartInputServer_ctrl->SetValue(B_CONTROL_ON);


	tabView->SetBorder(B_NO_BORDER);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(0, B_USE_DEFAULT_SPACING, 0, B_USE_DEFAULT_SPACING)
		.Add(tabView)
		.Add(new BSeparatorView(B_HORIZONTAL))
		.AddGroup(B_HORIZONTAL)
			.Add(restartInputServer_ctrl)
			.AddGlue()
			.Add(new BButton("cancelButton", "Cancel",
		new BMessage(MSG_CANCEL), B_WILL_DRAW))
			.Add(new BButton("okButton", "OK",
		new BMessage(MSG_OK), B_WILL_DRAW))
			.SetInsets(B_USE_WINDOW_SPACING, B_USE_DEFAULT_SPACING,
				B_USE_DEFAULT_SPACING, 0);

	updateControlsFromConfiguration();

}

void MainWindow::setCommandControl( int commandidx )
{
	swallowClick_ctrl->SetValue(configuration.swallowClick[commandidx]?B_CONTROL_ON:B_CONTROL_OFF);
	command_ctrl->SetText(configuration.buttonDownCommand[commandidx].command);
}

void MainWindow::preserveCommandControl( int commandidx )
{
	configuration.swallowClick[commandidx]=( swallowClick_ctrl->Value() == B_CONTROL_ON );

	ButtonDownCommand buttonDownCommand( command_ctrl->Text() ); 
	configuration.buttonDownCommand[commandidx]=buttonDownCommand;
}

void MainWindow::updateControlsFromConfiguration( void )
{
	char str[ MAX_CONF_STR ];
	int i;
	
	// Scrolling --------------------------------------------
	
	for (i=1;i<8;i++)
    {
		scrollWhenMouseDown_ctrl[i]->SetValue( configuration.scrollWhenMouseDown[i] ? B_CONTROL_ON : B_CONTROL_OFF );
	}

   	snprintf(str, MAX_CONF_STR, "%d", (int) sqrt(configuration.minScroll) );
    minScroll_ctrl->SetText(str);
    
	// Scroll Speed -----------------------------------------

	useWheelFactors_ctrl->SetValue( configuration.useWheelFactors ? B_CONTROL_ON : B_CONTROL_OFF );

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorX[0] );
    wheelFactorX_ctrl[0]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorY[0] );
    wheelFactorY_ctrl[0]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorX[1] );
    wheelFactorX_ctrl[1]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorY[1] );
    wheelFactorY_ctrl[1]->SetText(str);

   // Clicks ------------------------------------------------
   
	setCommandControl( 0 );
}	

bool MainWindow::QuitRequested()
{
       be_app->PostMessage(B_QUIT_REQUESTED);
       return(true);
}

void MainWindow::preserveSettings(void)
{
	int i;

	// Scrolling --------------------------------------------
	
	for (i=1;i<8;i++)
    {
		configuration.scrollWhenMouseDown[i] = ( scrollWhenMouseDown_ctrl[i]->Value() == B_CONTROL_ON );
	}

	float tmpFloat;
	if ( sscanf(minScroll_ctrl->Text(),"%f",&tmpFloat) == 1 )
	{
		configuration.minScroll = (int) (tmpFloat * tmpFloat);
	}

	// Scroll Speed -----------------------------------------

	configuration.useWheelFactors = ( useWheelFactors_ctrl->Value() == B_CONTROL_ON );

	if ( sscanf(wheelFactorX_ctrl[0]->Text(),"%f",&tmpFloat) == 1 )
	{
		configuration.wheelFactorX[0] = tmpFloat;
	}

	if ( sscanf(wheelFactorY_ctrl[0]->Text(),"%f",&tmpFloat) == 1 )
	{
		configuration.wheelFactorY[0] = tmpFloat;
	}

	if ( sscanf(wheelFactorX_ctrl[1]->Text(),"%f",&tmpFloat) == 1 )
	{
		configuration.wheelFactorX[1] = tmpFloat;
	}

	if ( sscanf(wheelFactorY_ctrl[1]->Text(),"%f",&tmpFloat) == 1 )
	{
		configuration.wheelFactorY[1] = tmpFloat;
	}

    // Clicks ------------------------------------------------

	// Clicks to configuration is managed via MessageReceived Function
	
   
    // Actual storing of configuration -----------------------

	configuration.save();

    // Restarting input server if requested ------------------
	if ( restartInputServer_ctrl->Value() == B_CONTROL_ON )
	{
		system("/system/servers/input_server -q");
	}
}

void MainWindow::MessageReceived(BMessage* message)
{
	static int previousCommandIndex=0;
	switch(message->what)
	{
		case CHANGEEVENT_MSG+0:
		case CHANGEEVENT_MSG+1:
		case CHANGEEVENT_MSG+2:
		case CHANGEEVENT_MSG+3:
		case CHANGEEVENT_MSG+4:
		case CHANGEEVENT_MSG+5:
		case CHANGEEVENT_MSG+6:
		case CHANGEEVENT_MSG+7:
		case CHANGEEVENT_MSG+8:
			preserveCommandControl( previousCommandIndex );
			setCommandControl( previousCommandIndex = message->what-CHANGEEVENT_MSG );
			break;
		
		case LEFT_MSG:
			command_ctrl->SetText(LEFT);
			break;

		case MIDDLE_MSG:
			command_ctrl->SetText(MIDDLE);
			break;

		case RIGHT_MSG:
			command_ctrl->SetText(RIGHT);
			break;

		case LEFTDBL_MSG:
			command_ctrl->SetText(LEFTDBL);
			break;

		case MIDDLEDBL_MSG:
			command_ctrl->SetText(MIDDLEDBL);
			break;

		case RIGHTDBL_MSG:
			command_ctrl->SetText(RIGHTDBL);
			break;
	
		case COPY_MSG:
			command_ctrl->SetText(COPY);
			break;

		case PASTE_MSG:
			command_ctrl->SetText(PASTE);
			break;

		case CUT_MSG:
			command_ctrl->SetText(CUT);
			break;
		
		case MSG_OK:
			preserveCommandControl( previousCommandIndex );
			preserveSettings();
			QuitRequested();
			break;
	
		case MSG_CANCEL:
			QuitRequested();
			break;

		default:
			BWindow::MessageReceived(message);
	}
}
