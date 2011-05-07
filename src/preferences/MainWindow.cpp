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

BTextControl *wheelFactorXView[2],*wheelFactorYView[2];
BCheckBox *useWheelFactorsView,*mousedownView[8],*swallowClickView;
BTextControl *eventcommand;

BTextControl *minimumMView,*commandView[2];
BCheckBox *restartinputserver;

#define MAX_CONF_STR 250

float wheelFactorX[2],wheelFactorY[2];
int minimumM, passthrough,passthroughasdouble;
char command[9][MAX_COMMAND_LENGTH+1];
bool useWheelFactors,swallowClick[9];
int primdbltime,secdbltime,tertdbltime;

int commandidx=0;

BButton *OKButton, *CancelButton;


MainWindow::MainWindow(BRect frame)
				: BWindow(frame, "UniversalScroller - Preferences", B_TITLED_WINDOW, B_NOT_RESIZABLE )
{
	BView *view;
	BTabView *tabview;
	BTab *tab;		
		
	tabview=new BTabView( BRect(0,0,630,300),"tabview");

    BRect tabViewFrame=BRect( 0, 0, 630, 300 );		
	tabViewFrame.InsetBy(5,5);
	tabViewFrame.bottom-=tabview->TabHeight();
	
	//
	// S C R O L L V I E W
	//
	view=new BView(tabViewFrame,"scrollview", B_FOLLOW_ALL, 0 );
    view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 

	view->AddChild( new BStringView( BRect(0,5,210,25), NULL, "Use UniversalScrolling for") );
	view->AddChild( new BStringView( BRect(0,25,210,45), NULL, "those combinations of buttons:") );

	mousedownView[1]=new BCheckBox(BRect(210,30,500,45),"PT","left",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[1]);
	mousedownView[3]=new BCheckBox(BRect(210,50,500,65),"PT","left+right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[3]);
	mousedownView[5]=new BCheckBox(BRect(210,70,500,85),"PT","left+middle",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[5]);
	mousedownView[7]=new BCheckBox(BRect(210,90,500,105),"PT","left+middle+right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[7]);
	mousedownView[4]=new BCheckBox(BRect(210,110,500,125),"PT","middle",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[4]);
	mousedownView[6]=new BCheckBox(BRect(210,130,500,145),"PT","middle+right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[6]);
	mousedownView[2]=new BCheckBox(BRect(210,150,500,165),"PT","right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(mousedownView[2]);

	view->AddChild(new BStringView(BRect(0,170,210,190), NULL, "Minimal mouse movement to"));
	view->AddChild(new BStringView(BRect(0,190,210,215), NULL, "start UniversalScrolling:"));
	minimumMView=new BTextControl(BRect(210,190,310,215), NULL, "","0",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	minimumMView->SetDivider(0);
	view->AddChild(minimumMView);


	
	tab=new BTab();
	tabview->AddTab(view,tab);
	tab->SetLabel("Scrolling");	
	
	//
	// S P E E D V I E W
	//
	view=new BView(tabViewFrame,"speedview",0,0);
    view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 

	useWheelFactorsView=new BCheckBox(BRect(0,5,500,25), NULL, "Use below factors for scroll-wheels",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(useWheelFactorsView);

	view->AddChild(new BStringView(BRect(20,35,500,55), NULL, "normal Scroll-Speed"));

	wheelFactorXView[0]=new BTextControl(BRect(40,55,200,75), NULL, "Factor X","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	wheelFactorXView[0]->SetDivider(70);
	view->AddChild(wheelFactorXView[0]);

	wheelFactorYView[0]=new BTextControl(BRect(40,75,200,95), NULL, "Factor Y","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	wheelFactorYView[0]->SetDivider(70);
	view->AddChild(wheelFactorYView[0]);

	view->AddChild(new BStringView(BRect(20,105,500,125), NULL, "fast Scroll-Speed"));

	wheelFactorXView[1]=new BTextControl(BRect(40,125,200,145), NULL, "Factor X","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	wheelFactorXView[1]->SetDivider(70);
	view->AddChild(wheelFactorXView[1]);

	wheelFactorYView[1]=new BTextControl(BRect(40,145,200,165), NULL, "Factor Y","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	wheelFactorYView[1]->SetDivider(70);
	view->AddChild(wheelFactorYView[1]);

	tab=new BTab();
	tabview->AddTab(view,tab);
	tab->SetLabel("Scroll-Speed");	
	
	//
	// C L I C K V I E W
	//
	view=new BView(tabViewFrame,"clickview",0,0);
    view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 
	view->AddChild(new BStringView(BRect(10,10,300,25),NULL,"What to do when ..."));
	
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
	menuField = new BMenuField( BRect( 20, 30, 340, 55 ), NULL, NULL, menu );
	
	view->AddChild( menuField );
	
	swallowClickView=new BCheckBox(BRect(20,70,500,90), NULL, "swallow this click", NULL, B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW|B_NAVIGABLE);
	view->AddChild(swallowClickView);

	eventcommand=new BTextControl(BRect(10,95,340,115), NULL, "", "", NULL);
	eventcommand->SetDivider(0);
	view->AddChild(eventcommand);
	
	view->AddChild(new BButton(BRect( 10,125,120,150), NULL, "Left Sgl Click",new BMessage(LEFT_MSG)));
	view->AddChild(new BButton(BRect(120,125,230,150), NULL, "Middle Sgl Click",new BMessage(MIDDLE_MSG)));
	view->AddChild(new BButton(BRect(230,125,340,150), NULL, "Right Sgl Click",new BMessage(RIGHT_MSG)));
	
	view->AddChild(new BButton(BRect( 10,150,120,175), NULL, "Left Dbl Click",new BMessage(LEFTDBL_MSG)));
	view->AddChild(new BButton(BRect(120,150,230,175), NULL, "Middle Dbl Click",new BMessage(MIDDLEDBL_MSG)));
	view->AddChild(new BButton(BRect(230,150,340,175), NULL, "Right Dbl Click",new BMessage(RIGHTDBL_MSG)));

	view->AddChild(new BButton(BRect( 10,175,120,200), NULL, "Cut",new BMessage(CUT_MSG)));
	view->AddChild(new BButton(BRect(120,175,230,200), NULL, "Copy",new BMessage(COPY_MSG)));
	view->AddChild(new BButton(BRect(230,175,340,200), NULL, "Paste",new BMessage(PASTE_MSG)));


	tab=new BTab();
	tabview->AddTab(view,tab);
	tab->SetLabel("Clicks");	
	

	AddChild(tabview);

    // Button Panel at bottom ----------------------------------------------------------------
	BView *ButtonPanelBottom=new BView(BRect(0,300,630,325), NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, 0 );
    ButtonPanelBottom->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 
       	
	ButtonPanelBottom->AddChild(new BButton(BRect(0,0,60,25), NULL,"Cancel",new BMessage(MSG_CANCEL)));

	restartinputserver=new BCheckBox(BRect(120,0,310,25), NULL, "restart inputserver on OK",NULL,B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW|B_NAVIGABLE);
    restartinputserver->SetValue(B_CONTROL_ON);
	ButtonPanelBottom->AddChild( restartinputserver );

	ButtonPanelBottom->AddChild(new BButton(BRect(310,0,370,25), NULL,"OK",new BMessage(MSG_OK),B_FOLLOW_RIGHT|B_FOLLOW_TOP));
		
	AddChild(ButtonPanelBottom);

	updateControlsFromConfiguration();

}

static void downCommandChanger( int commandidx )
{
	swallowClickView->SetValue(swallowClick[commandidx]?B_CONTROL_ON:B_CONTROL_OFF);
	eventcommand->SetText(command[commandidx]);
}

void MainWindow::updateControlsFromConfiguration( void )
{
	char str[ MAX_CONF_STR+1 ];
	int i;
	
	// Scrolling --------------------------------------------
	
	for (i=1;i<8;i++)
    {
		mousedownView[i]->SetValue( configuration.scrollWhenMouseDown[i] ? B_CONTROL_ON : B_CONTROL_OFF );
	}

   	snprintf(str, MAX_CONF_STR, "%d", (int) sqrt(configuration.minScroll) );
    minimumMView->SetText(str);
    
	// Scroll Speed -----------------------------------------

	useWheelFactorsView->SetValue( configuration.useWheelFactors ? B_CONTROL_ON : B_CONTROL_OFF );

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorX[0] );
    wheelFactorXView[0]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorY[0] );
    wheelFactorYView[0]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorX[1] );
    wheelFactorXView[1]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.wheelFactorY[1] );
    wheelFactorYView[1]->SetText(str);

   // Clicks ------------------------------------------------
   
	for (i=0;i<9;i++)
    {
		swallowClick[i]=configuration.swallowClick[i];
		strncpy( command[i], configuration.buttonDownCommand[i].command, MAX_COMMAND_LENGTH );
	}
	downCommandChanger(0);
}	

bool MainWindow::QuitRequested()
{
       be_app->PostMessage(B_QUIT_REQUESTED);
       return(true);
}

static void okClick(void)
{
	bool boolval;

			long i;
		
		
		return;	
			
			int handle=open("/boot/home/config/add-ons/input_server/filters/UniversalScroller",O_RDWR);


		swallowClick[commandidx]=(swallowClickView->Value()==B_CONTROL_ON);


    	//lseek(handle,OFFSETFACTORSFORWHEEL,0);
		boolval=(useWheelFactorsView->Value()==B_CONTROL_ON);
		write(handle,&boolval,sizeof(bool));

		for (i=1;i<8;i++)
	    {
	    	//lseek(handle,OFFSETSCROLLMOUSEDOWN+i,0);
			boolval=(mousedownView[i]->Value()==B_CONTROL_ON);
			write(handle,&boolval,sizeof(bool));
		}
		for (i=1;i<8;i++)
	    {
	    	//lseek(handle,OFFSETswallowClick+i,0);
			write(handle,&swallowClick[i],sizeof(bool));
		}
		
		sscanf(wheelFactorXView[0]->Text(),"%f",&wheelFactorX[0]);
		sscanf(wheelFactorYView[0]->Text(),"%f",&wheelFactorY[0]);
		sscanf(wheelFactorXView[1]->Text(),"%f",&wheelFactorX[1]);
		sscanf(wheelFactorYView[1]->Text(),"%f",&wheelFactorY[1]);
		sscanf(minimumMView->Text(),"%u",&minimumM);
		minimumM=minimumM*minimumM;


    	//lseek(handle,OFFSETFACTORX0,0);
		write(handle,&wheelFactorX[0],sizeof(float));

    	//lseek(handle,OFFSETFACTORY0,0);
		write(handle,&wheelFactorY[0],sizeof(float));

    	//lseek(handle,OFFSETFACTORX1,0);
		write(handle,&wheelFactorX[1],sizeof(float));

    	//lseek(handle,OFFSETFACTORY1,0);
		write(handle,&wheelFactorY[1],sizeof(float));

		//lseek(handle,OFFSETMINIMUM,0);
		write(handle,&minimumM,sizeof(int));

	    strcpy(command[commandidx],eventcommand->Text());
    	//lseek(handle,OFFSETCMD0,0);
		write(handle,command,255*9);
	    
			close(handle);
			if (restartinputserver->Value()==B_CONTROL_ON) system("/system/servers/input_server -q");
}

void MainWindow::MessageReceived(BMessage* message)
{
	
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
			downCommandChanger( message->what-CHANGEEVENT_MSG );
			break;
		
		case LEFT_MSG:
			eventcommand->SetText(LEFT);
			break;

		case MIDDLE_MSG:
			eventcommand->SetText(MIDDLE);
			break;

		case RIGHT_MSG:
			eventcommand->SetText(RIGHT);
			break;

		case LEFTDBL_MSG:
			eventcommand->SetText(LEFTDBL);
			break;

		case MIDDLEDBL_MSG:
			eventcommand->SetText(MIDDLEDBL);
			break;

		case RIGHTDBL_MSG:
			eventcommand->SetText(RIGHTDBL);
			break;
	
		case COPY_MSG:
			eventcommand->SetText(COPY);
			break;

		case PASTE_MSG:
			eventcommand->SetText(PASTE);
			break;

		case CUT_MSG:
			eventcommand->SetText(CUT);
			break;
		
		case MSG_OK:
			okClick();
			QuitRequested();
			break;
	
		case MSG_CANCEL:
			QuitRequested();
			break;

		default:
			BWindow::MessageReceived(message);
	}
}
