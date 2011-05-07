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
#define MSG_OK 'USOK'
#define CHANGEEVENT_MSG 'USC'*0x100
#define CHANGEEVENTTYPE_MSG 'USET'
#define LEFT_MSG 'USLS'
#define LEFTDBL_MSG 'USLD'
#define MIDDLE_MSG 'USMS'
#define MIDDLEDBL_MSG 'USMD'
#define RIGHT_MSG 'USRS'
#define RIGHTDBL_MSG 'USRD'

#define WORKSPACEM_MSG 'USW-'
#define WORKSPACE_MSG 'USW '
#define WORKSPACEP_MSG 'USW+'

#define CUT_MSG 'USCT'
#define COPY_MSG 'USCP'
#define PASTE_MSG 'USPT'

#include "command_strings.h"

#include "Configuration.h"

BTextControl *factorXView[2],*factorYView[2],*speedleftView,*speedmiddleView,*speedrightView;
BCheckBox *factorsforwheelView,*mousedownView[8],*swallowClickView;
BPopUpMenu *popupmenu;
BButton *eventbtn;
BTextControl *eventcommand;

BTextControl *minimumMView,*commandView[2],*workspaceView;
BCheckBox *passthroughView,*passthroughasdoubleView,*restartinputserver;

#define MAX_COMMAND_LENGTH 250

float factorX[2],factorY[2];
int minimumM, passthrough,passthroughasdouble;
char command[9][MAX_COMMAND_LENGTH+1];
bool factorsforwheel,swallowclick[9];
int primdbltime,secdbltime,tertdbltime;

int commandidx=0;

BButton *OKButton, *CancelButton;

class USPrefApplication : public BApplication 
{
public:
	USPrefApplication();
};



class USPrefWindow : public BWindow 
{
public:
				USPrefWindow(BRect frame); 
				virtual void	MessageReceived(BMessage* message);
				virtual	bool	QuitRequested();

	private:
		Configuration configuration;
				
		void updateControlsFromConfiguration( void );

				int LINES;

};
USPrefApplication::USPrefApplication()
		  		  : BApplication("application/x-vnd.TM.USPref")
{
	USPrefWindow		*aWindow;
	BRect			aRect;
	int left=50,top=50;
	aRect.Set(left, top, left+370, top+325);
	aWindow = new USPrefWindow(aRect);

	aWindow->Show();
}


USPrefWindow::USPrefWindow(BRect frame)
				: BWindow(frame, "UniversalScroller - Preferences", B_TITLED_WINDOW, B_NOT_RESIZABLE )
{
	BView *aView;
	BTabView *tabview;
	BTab *tab;		

	frame.left=0;
	frame.top=0;
	frame.bottom=300;
	frame.right=630;
		
	tabview=new BTabView(frame,"tabview");
		
	frame.InsetBy(5,5);
	frame.bottom-=tabview->TabHeight();
	
	//
	// S C R O L L V I E W
	//
	aView=new BView(frame,"scrollview", B_FOLLOW_ALL, 0 );
    aView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 

	aView->AddChild( new BStringView( BRect(0,5,210,25), NULL, "Use UniversalScrolling for") );
	aView->AddChild( new BStringView( BRect(0,25,210,45), NULL, "those combinations of buttons:") );

	mousedownView[1]=new BCheckBox(BRect(210,30,500,45),"PT","left",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[1]);
	mousedownView[3]=new BCheckBox(BRect(210,50,500,65),"PT","left+right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[3]);
	mousedownView[5]=new BCheckBox(BRect(210,70,500,85),"PT","left+middle",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[5]);
	mousedownView[7]=new BCheckBox(BRect(210,90,500,105),"PT","left+middle+right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[7]);
	mousedownView[4]=new BCheckBox(BRect(210,110,500,125),"PT","middle",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[4]);
	mousedownView[6]=new BCheckBox(BRect(210,130,500,145),"PT","middle+right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[6]);
	mousedownView[2]=new BCheckBox(BRect(210,150,500,165),"PT","right",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[2]);

	aView->AddChild(new BStringView(BRect(0,170,210,190),"SOME","Minimal mouse movement to"));
	aView->AddChild(new BStringView(BRect(0,190,210,215),"SOME","start UniversalScrolling:"));
	minimumMView=new BTextControl(BRect(210,190,310,215),"MM","","0",NULL,B_FOLLOW_LEFT,B_WILL_DRAW|B_NAVIGABLE);
	minimumMView->SetDivider(0);
	aView->AddChild(minimumMView);


	
	tab=new BTab();
	tabview->AddTab(aView,tab);
	tab->SetLabel("Scrolling");	
	
	//
	// S P E E D V I E W
	//
	aView=new BView(frame,"speedview",0,0);
    aView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 

	factorsforwheelView=new BCheckBox(BRect(0,5,500,25),"PT","Use below factors for scroll-wheels",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(factorsforwheelView);

	aView->AddChild(new BStringView(BRect(20,35,500,55),"SOME","normal Scroll-Speed"));

	factorXView[0]=new BTextControl(BRect(40,55,200,75),"X0","Factor X","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorXView[0]->SetDivider(70);
	aView->AddChild(factorXView[0]);

	factorYView[0]=new BTextControl(BRect(40,75,200,95),"Y1","Factor Y","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorYView[0]->SetDivider(70);
	aView->AddChild(factorYView[0]);

	aView->AddChild(new BStringView(BRect(20,105,500,125),"SOME","fast Scroll-Speed"));

	factorXView[1]=new BTextControl(BRect(40,125,200,145),"X1","Factor X","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorXView[1]->SetDivider(70);
	aView->AddChild(factorXView[1]);

	factorYView[1]=new BTextControl(BRect(40,145,200,165),"Y1","Factor Y","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorYView[1]->SetDivider(70);
	aView->AddChild(factorYView[1]);

/*	aView->AddChild(new BStringView(BRect(170,10,320,25),"SOME","Double-click time (in millisecs)"));
	speedleftView=new BTextControl(BRect(190,30,320,65),"PrimButton","Primary Button","350",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	speedleftView->SetDivider(90);
	aView->AddChild(speedleftView);
	speedrightView=new BTextControl(BRect(190,30+20,320,65+20),"SecButton","Secondary Button","350",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	speedrightView->SetDivider(90);
	aView->AddChild(speedrightView);
	speedmiddleView=new BTextControl(BRect(190,30+40,320,65+40),"SecButton","Tertiary Button","350",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	speedmiddleView->SetDivider(90);
	aView->AddChild(speedmiddleView);
*/

	tab=new BTab();
	tabview->AddTab(aView,tab);
	tab->SetLabel("Scroll-Speed");	
	

	//
	// C L I C K V I E W
	//
	aView=new BView(frame,"clickview",0,0);
    aView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 
	aView->AddChild(new BStringView(BRect(10,10,300,25),NULL,"What to do when ..."));
	
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
	
	aView->AddChild( menuField );
	
	eventbtn=new BButton(BRect(20,30,340,55),"selmouse","left down",new BMessage(CHANGEEVENTTYPE_MSG));
	//aView->AddChild(eventbtn);
	
	swallowClickView=new BCheckBox(BRect(20,70,500,90),"PT","swallow this click",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(swallowClickView);

	eventcommand=new BTextControl(BRect(10,95,340,115),"EQU","","",NULL);
	eventcommand->SetDivider(0);
	aView->AddChild(eventcommand);
	
	aView->AddChild(new BButton(BRect( 10,125,120,150),"selmouse","Left Sgl Click",new BMessage(LEFT_MSG)));
	aView->AddChild(new BButton(BRect(120,125,230,150),"selmouse","Middle Sgl Click",new BMessage(MIDDLE_MSG)));
	aView->AddChild(new BButton(BRect(230,125,340,150),"selmouse","Right Sgl Click",new BMessage(RIGHT_MSG)));
	
	aView->AddChild(new BButton(BRect( 10,150,120,175),"selmouse","Left Dbl Click",new BMessage(LEFTDBL_MSG)));
	aView->AddChild(new BButton(BRect(120,150,230,175),"selmouse","Middle Dbl Click",new BMessage(MIDDLEDBL_MSG)));
	aView->AddChild(new BButton(BRect(230,150,340,175),"selmouse","Right Dbl Click",new BMessage(RIGHTDBL_MSG)));

	aView->AddChild(new BButton(BRect( 10,175,120,200),"selmouse","Cut",new BMessage(CUT_MSG)));
	aView->AddChild(new BButton(BRect(120,175,230,200),"selmouse","Copy",new BMessage(COPY_MSG)));
	aView->AddChild(new BButton(BRect(230,175,340,200),"selmouse","Paste",new BMessage(PASTE_MSG)));


	tab=new BTab();
	tabview->AddTab(aView,tab);
	tab->SetLabel("Clicks");	
	

	AddChild(tabview);

    // Button Panel at bottom ----------------------------------------------------------------
	BView *ButtonPanelBottom=new BView(BRect(0,300,630,325), NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM, 0 );
    ButtonPanelBottom->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR)); 
       	
	ButtonPanelBottom->AddChild(new BButton(BRect(0,0,60,25), NULL,"Cancel",new BMessage(MSG_CANCEL)));

	restartinputserver=new BCheckBox(BRect(120,0,310,25),"PT","restart inputserver on OK",NULL,B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP,B_WILL_DRAW|B_NAVIGABLE);
    restartinputserver->SetValue(B_CONTROL_ON);
	ButtonPanelBottom->AddChild( restartinputserver );

	ButtonPanelBottom->AddChild(new BButton(BRect(310,0,370,25), NULL,"OK",new BMessage(MSG_OK),B_FOLLOW_RIGHT|B_FOLLOW_TOP));
		
	AddChild(ButtonPanelBottom);

	updateControlsFromConfiguration();

}

#define MAX_CONF_STR 250

static void downCommandChanger( int commandidx )
{
	swallowClickView->SetValue(swallowclick[commandidx]?B_CONTROL_ON:B_CONTROL_OFF);
	eventcommand->SetText(command[commandidx]);
}

void USPrefWindow::updateControlsFromConfiguration( void )
{
	char str[ MAX_CONF_STR+1 ];
	int i;
	
	// Scrolling --------------------------------------------
	
	for (i=1;i<8;i++)
    {
		mousedownView[i]->SetValue( configuration.scrollmousedown[i] ? B_CONTROL_ON : B_CONTROL_OFF );
	}

   	snprintf(str, MAX_CONF_STR, "%d", (int) sqrt(configuration.minScroll) );
    minimumMView->SetText(str);
    
	// Scroll Speed -----------------------------------------

	factorsforwheelView->SetValue( configuration.factorsforwheel ? B_CONTROL_ON : B_CONTROL_OFF );

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.factorX[0] );
    factorXView[0]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.factorY[0] );
    factorYView[0]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.factorX[1] );
    factorXView[1]->SetText(str);

   	snprintf(str, MAX_CONF_STR, "%.2f", configuration.factorY[1] );
    factorYView[1]->SetText(str);

   // Clicks ------------------------------------------------
   
	for (i=0;i<9;i++)
    {
		swallowclick[i]=configuration.swallowclick[i];
		strncpy( command[i], configuration.buttonDownCommand[i].command, MAX_COMMAND_LENGTH );
	}
	downCommandChanger(0);
}	

bool USPrefWindow::QuitRequested()
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


		swallowclick[commandidx]=(swallowClickView->Value()==B_CONTROL_ON);


    	//lseek(handle,OFFSETFACTORSFORWHEEL,0);
		boolval=(factorsforwheelView->Value()==B_CONTROL_ON);
		write(handle,&boolval,sizeof(bool));

		for (i=1;i<8;i++)
	    {
	    	//lseek(handle,OFFSETSCROLLMOUSEDOWN+i,0);
			boolval=(mousedownView[i]->Value()==B_CONTROL_ON);
			write(handle,&boolval,sizeof(bool));
		}
		for (i=1;i<8;i++)
	    {
	    	//lseek(handle,OFFSETSWALLOWCLICK+i,0);
			write(handle,&swallowclick[i],sizeof(bool));
		}
		
		sscanf(factorXView[0]->Text(),"%f",&factorX[0]);
		sscanf(factorYView[0]->Text(),"%f",&factorY[0]);
		sscanf(factorXView[1]->Text(),"%f",&factorX[1]);
		sscanf(factorYView[1]->Text(),"%f",&factorY[1]);
		sscanf(minimumMView->Text(),"%u",&minimumM);
		minimumM=minimumM*minimumM;


    	//lseek(handle,OFFSETFACTORX0,0);
		write(handle,&factorX[0],sizeof(float));

    	//lseek(handle,OFFSETFACTORY0,0);
		write(handle,&factorY[0],sizeof(float));

    	//lseek(handle,OFFSETFACTORX1,0);
		write(handle,&factorX[1],sizeof(float));

    	//lseek(handle,OFFSETFACTORY1,0);
		write(handle,&factorY[1],sizeof(float));

		//lseek(handle,OFFSETMINIMUM,0);
		write(handle,&minimumM,sizeof(int));

	    strcpy(command[commandidx],eventcommand->Text());
    	//lseek(handle,OFFSETCMD0,0);
		write(handle,command,255*9);
	    
			close(handle);
			if (restartinputserver->Value()==B_CONTROL_ON) system("/system/servers/input_server -q");
}

void USPrefWindow::MessageReceived(BMessage* message)
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

int main(int argc, char*argv[])
{	
	USPrefApplication	myApplication;
	myApplication.Run();

	return(0);
}
