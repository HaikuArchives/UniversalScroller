/*#define OFFSETFACTORX0 				11520
#define OFFSETFACTORX1 				OFFSETFACTORX0+4
#define OFFSETFACTORY0 				OFFSETFACTORX0+8
#define OFFSETFACTORY1  			OFFSETFACTORX0+12
#define OFFSETMINIMUM      			OFFSETFACTORX0+16
#define OFFSETPASSTHROUGH  			OFFSETFACTORX0+20
#define OFFSETPASSTHROUGHASDOUBLE  	OFFSETFACTORX0+24
#define OFFSETCMD0         			OFFSETFACTORX0+32
#define OFFSETCMD1         			OFFSETCMD0+255
*/
#include "offsets.h"
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
#define MSG_HOMEPAGE 'USHP'
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

#include "../include/command_strings.h"

BTextControl *factorXView[2],*factorYView[2],*speedleftView,*speedmiddleView,*speedrightView;
BCheckBox *factorsforwheelView,*mousedownView[8],*swallowClickView;
BPopUpMenu *popupmenu;
BButton *eventbtn;
BTextControl *eventcommand;

BTextControl *minimumMView,*commandView[2],*workspaceView;
BCheckBox *passthroughView,*passthroughasdoubleView,*restartinputserver;

float factorX[2],factorY[2];
int minimumM, passthrough,passthroughasdouble;
char command[9][255];
bool factorsforwheel,swallowclick[8];
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


				int LINES;
};
USPrefApplication::USPrefApplication()
		  		  : BApplication("application/x-vnd.TM.USPref")
{
	USPrefWindow		*aWindow;
	BRect			aRect;
	int left=50,top=50;
	aRect.Set(left, top, left+330, top+225);
	aWindow = new USPrefWindow(aRect);

	aWindow->Show();
}


USPrefWindow::USPrefWindow(BRect frame)
				: BWindow(frame, "UniversalScroller - Preferences", B_TITLED_WINDOW,B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	BView *aView;
	BTabView *tabview;
	BTab *tab;
	int i;
	int handle;
	char str[64];
	bool boolval;

		popupmenu=new BPopUpMenu("");
		popupmenu->AddItem(new BMenuItem("left down",new BMessage(CHANGEEVENT_MSG+0)));                           
		popupmenu->AddItem(new BMenuItem("left down and right interclick",new BMessage(CHANGEEVENT_MSG+1)));
		popupmenu->AddItem(new BMenuItem("left down and middle interclick",new BMessage(CHANGEEVENT_MSG+2)));
		popupmenu->AddItem(new BMenuItem("right down",new BMessage(CHANGEEVENT_MSG+3)));
		popupmenu->AddItem(new BMenuItem("right down and left interclick",new BMessage(CHANGEEVENT_MSG+4)));
		popupmenu->AddItem(new BMenuItem("right down and middle interclick",new BMessage(CHANGEEVENT_MSG+5)));
		popupmenu->AddItem(new BMenuItem("middle down",new BMessage(CHANGEEVENT_MSG+6)));
		popupmenu->AddItem(new BMenuItem("middle down and left interclick",new BMessage(CHANGEEVENT_MSG+7)));
		popupmenu->AddItem(new BMenuItem("middle down and right interclick",new BMessage(CHANGEEVENT_MSG+8)));

	frame.left=frame.top=0;
	frame.bottom=200;
	frame.right=330;
		
	tabview=new BTabView(frame,"tabview");

	
	frame.InsetBy(5,5);
	frame.bottom-=tabview->TabHeight();
	
	//
	// S C R O L L V I E W
	//
	aView=new BView(frame,"scrollview",0,0);
	aView->SetViewColor(0xd8,0xd8,0xd8);

	aView->AddChild(new BStringView(BRect(10,10,300,25),"SOME","use UniversalScrolling when these buttons are down"));
	mousedownView[1]=new BCheckBox(BRect(100,30,180,45),"PT","left",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[1]);
	mousedownView[3]=new BCheckBox(BRect(100,50,180,65),"PT","left+right",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[3]);
	mousedownView[5]=new BCheckBox(BRect(100,70,180,85),"PT","left+middle",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[5]);
	mousedownView[7]=new BCheckBox(BRect(100,90,210,105),"PT","left+middle+right",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[7]);
	mousedownView[4]=new BCheckBox(BRect(100,110,180,125),"PT","middle",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[4]);
	mousedownView[6]=new BCheckBox(BRect(100,130,180,145),"PT","middle+right",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[6]);
	mousedownView[2]=new BCheckBox(BRect(100,150,180,165),"PT","right",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(mousedownView[2]);

	aView->AddChild(new BStringView(BRect(225,110,310,135),"SOME","min. Movement"));
	aView->AddChild(new BStringView(BRect(230,123,310,148),"SOME","for Scrolling"));
	minimumMView=new BTextControl(BRect(210,150,310,165),"MM","","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	minimumMView->SetDivider(0);
	aView->AddChild(minimumMView);


	
	tab=new BTab();
	tabview->AddTab(aView,tab);
	tab->SetLabel("Scrolling");	
	
	//
	// S P E E D V I E W
	//
	aView=new BView(frame,"speedview",0,0);
	aView->SetViewColor(0xd8,0xd8,0xd8);
	aView->AddChild(new BStringView(BRect(10,10,150,25),"SOME","normal Scroll-Speed"));
	factorXView[0]=new BTextControl(BRect(20,30,130,45),"X0","Factor X","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorXView[0]->SetDivider(50);
	aView->AddChild(factorXView[0]);
	factorYView[0]=new BTextControl(BRect(20,50,130,65),"Y1","Factor Y","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorYView[0]->SetDivider(50);
	aView->AddChild(factorYView[0]);
	aView->AddChild(new BStringView(BRect(10,80+10,150,80+25),"SOME","fast Scroll-Speed"));
	factorXView[1]=new BTextControl(BRect(20,80+30,130,80+45),"X1","Factor X","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorXView[1]->SetDivider(50);
	aView->AddChild(factorXView[1]);
	factorYView[1]=new BTextControl(BRect(20,80+50,130,80+65),"Y1","Factor Y","0",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	factorYView[1]->SetDivider(50);
	aView->AddChild(factorYView[1]);
	((BTextView *)factorXView[0])->SetWordWrap(false);
	((BTextView *)factorYView[0])->SetWordWrap(false);
	((BTextView *)factorXView[1])->SetWordWrap(false);
	((BTextView *)factorYView[1])->SetWordWrap(false);

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
	factorsforwheelView=new BCheckBox(BRect(10,155,180,170),"PT","Use factors for scroll-wheels",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(factorsforwheelView);

	tab=new BTab();
	tabview->AddTab(aView,tab);
	tab->SetLabel("Speed");	
	

	//
	// C L I C K V I E W
	//
	aView=new BView(frame,"clickview",0,0);
	aView->SetViewColor(0xd8,0xd8,0xd8);
	aView->AddChild(new BStringView(BRect(10,10,300,25),"SOME","What to do when ..."));
	eventbtn=new BButton(BRect(20,30,180,55),"selmouse","left down",new BMessage(CHANGEEVENTTYPE_MSG));
	aView->AddChild(eventbtn);
	
	eventcommand=new BTextControl(BRect(10,70,310,85),"EQU","","",NULL);
	eventcommand->SetDivider(0);
	aView->AddChild(eventcommand);
	
	aView->AddChild(new BButton(BRect(10,95,110,110),"selmouse","Left Click",new BMessage(LEFT_MSG)));
	aView->AddChild(new BButton(BRect(110,95,210,110),"selmouse","Middle Click",new BMessage(MIDDLE_MSG)));
	aView->AddChild(new BButton(BRect(210,95,310,110),"selmouse","Right Click",new BMessage(RIGHT_MSG)));
	
	aView->AddChild(new BButton(BRect(10,119,110,130),"selmouse","Left Double Click",new BMessage(LEFTDBL_MSG)));
	aView->AddChild(new BButton(BRect(110,119,210,130),"selmouse","Middle Double Click",new BMessage(MIDDLEDBL_MSG)));
	aView->AddChild(new BButton(BRect(210,119,310,130),"selmouse","Right Double Click",new BMessage(RIGHTDBL_MSG)));

	aView->AddChild(new BButton(BRect(10,143,110,160),"selmouse","Cut",new BMessage(CUT_MSG)));
	aView->AddChild(new BButton(BRect(110,143,210,160),"selmouse","Copy",new BMessage(COPY_MSG)));
	aView->AddChild(new BButton(BRect(210,143,310,160),"selmouse","Paste",new BMessage(PASTE_MSG)));

	swallowClickView=new BCheckBox(BRect(210,40,310,60),"PT","swallow this click",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
	aView->AddChild(swallowClickView);

	tab=new BTab();
	tabview->AddTab(aView,tab);
	tab->SetLabel("Clicks");	
	

	AddChild(tabview);
	
	AddChild(new BButton(BRect(0,200,60,225), "EQUBTN","Cancel",new BMessage(MSG_CANCEL)));
	AddChild(new BButton(BRect(60,200,120,225), "EQUBTN","Homepage",new BMessage(MSG_HOMEPAGE)));
	AddChild(new BButton(BRect(270,200,330,225), "EQUBTN","OK",new BMessage(MSG_OK)));
	restartinputserver=new BCheckBox(BRect(120,200,270,225),"PT","restart inputserver on OK",NULL,B_FOLLOW_LEFT_RIGHT,B_WILL_DRAW|B_NAVIGABLE);
    restartinputserver->SetValue(B_CONTROL_ON);
    restartinputserver->SetViewColor(0xd8,0xd8,0xd8);
    restartinputserver->SetLowColor(0xd8,0xd8,0xd8);
	AddChild(restartinputserver);

	
	handle=open("/boot/home/config/add-ons/input_server/filters/UniversalScroller",O_RDWR);
	if (handle<=0)
	{
		system("alert \"UniversalScroller does not seem to be installed.\n\nInstalling it.\" \"OK\"");
		system("install.sh");
		handle=open("/boot/home/config/add-ons/input_server/filters/UniversalScroller",O_RDWR);
	}
	if (handle>0)
	{
	    minimumMView->SetText("0");

  /*  	lseek(handle,OFFSETTIMEPRIMARY,0);
		read(handle,&primdbltime,sizeof(int));
		primdbltime/=1000;
		sprintf(str,"%d",primdbltime);
	    speedleftView->SetText(str);

    	lseek(handle,OFFSETTIMESECONDARY,0);
		read(handle,&secdbltime,sizeof(int));
		secdbltime/=1000;
		sprintf(str,"%d",secdbltime);
	    speedrightView->SetText(str);

    	lseek(handle,OFFSETTIMETERTIARY,0);
		read(handle,&tertdbltime,sizeof(int));
		tertdbltime/=1000;
		sprintf(str,"%d",tertdbltime);
	    speedmiddleView->SetText(str);
*/
    	lseek(handle,OFFSETMINIMUM,0);
		read(handle,&minimumM,sizeof(int));
		minimumM=sqrt(minimumM);
		sprintf(str,"%d",minimumM);
	    minimumMView->SetText(str);


    	lseek(handle,OFFSETFACTORSFORWHEEL,0);
		read(handle,&boolval,sizeof(bool));
		if (boolval)
			factorsforwheelView->SetValue(B_CONTROL_ON);
    	else
			factorsforwheelView->SetValue(B_CONTROL_OFF);

		for (i=1;i<8;i++)
	    {
	    	lseek(handle,OFFSETSCROLLMOUSEDOWN+i,0);
			read(handle,&boolval,sizeof(bool));
			if (boolval)
				mousedownView[i]->SetValue(B_CONTROL_ON);
	    	else
				mousedownView[i]->SetValue(B_CONTROL_OFF);
		}

		for (i=1;i<8;i++)
	    {
	    	lseek(handle,OFFSETSWALLOWCLICK+i,0);
			read(handle,&swallowclick[i],sizeof(bool));
		}

		if (swallowclick[0])
			mousedownView[i]->SetValue(B_CONTROL_ON);
    	else
			mousedownView[i]->SetValue(B_CONTROL_OFF);

    	lseek(handle,OFFSETFACTORX0,0);
		read(handle,&factorX[0],sizeof(float));
		sprintf(str,"%f",factorX[0]);
	    factorXView[0]->SetText(str);

    	lseek(handle,OFFSETFACTORY0,0);
		read(handle,&factorY[0],sizeof(float));
		sprintf(str,"%f",factorY[0]);
	    factorYView[0]->SetText(str);

    	lseek(handle,OFFSETFACTORX1,0);
		read(handle,&factorX[1],sizeof(float));
		sprintf(str,"%f",factorX[1]);
	    factorXView[1]->SetText(str);

    	lseek(handle,OFFSETFACTORY1,0);
		read(handle,&factorY[1],sizeof(float));
		sprintf(str,"%f",factorY[1]);
	    factorYView[1]->SetText(str);

    	lseek(handle,OFFSETCMD0,0);
		read(handle,&command[0],255*9);
	    eventcommand->SetText(command[0]);


	}


}


bool USPrefWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return(true);
}
void USPrefWindow::MessageReceived(BMessage* message)
{
	char str[63];
	long pos;
	int i;
	BMenuItem *item;
	bool boolval;



	switch(message->what)
	{
/*		case MSG_README:
			sprintf(str,"StyledEdit %sreadme.txt",path);
			system(str);
		break;
*/
		case CHANGEEVENTTYPE_MSG:
			item=popupmenu->Go(BPoint(50+Frame().left,Frame().top+30));
			if (item!=NULL)
			{
				swallowclick[commandidx]=(swallowClickView->Value()==B_CONTROL_ON);
				strcpy(command[commandidx],eventcommand->Text());
				for (i=0;i<9;i++)
					printf("%s\n",command[i]);
				printf("%d\n",commandidx);
				eventbtn->SetLabel(item->Label());
				commandidx=popupmenu->IndexOf(item);
				printf("%d\n",commandidx);
				eventcommand->SetText(command[commandidx]);
				swallowClickView->SetValue(swallowclick[commandidx]?B_CONTROL_ON:B_CONTROL_OFF);
			}
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
		case WORKSPACEM_MSG:
			eventcommand->SetText("workspace -");
		break;
		case WORKSPACE_MSG:
			sprintf(str,"workspace %s",workspaceView->Text());
			eventcommand->SetText(str);
		break;
		case WORKSPACEP_MSG:
			eventcommand->SetText("workspace +");
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
		
		case MSG_HOMEPAGE:
			system("NetPositive http://come.to/Troublemaker");
		break;
		
		case MSG_OK:
			long i,j;
			int handle;
			float val;
			int vz;

			
			handle=open("/boot/home/config/add-ons/input_server/filters/UniversalScroller",O_RDWR);


		swallowclick[commandidx]=(swallowClickView->Value()==B_CONTROL_ON);


    	lseek(handle,OFFSETFACTORSFORWHEEL,0);
		boolval=(factorsforwheelView->Value()==B_CONTROL_ON);
		write(handle,&boolval,sizeof(bool));

		for (i=1;i<8;i++)
	    {
	    	lseek(handle,OFFSETSCROLLMOUSEDOWN+i,0);
			boolval=(mousedownView[i]->Value()==B_CONTROL_ON);
			write(handle,&boolval,sizeof(bool));
		}
		for (i=1;i<8;i++)
	    {
	    	lseek(handle,OFFSETSWALLOWCLICK+i,0);
			write(handle,&swallowclick[i],sizeof(bool));
		}
		
		sscanf(factorXView[0]->Text(),"%f",&factorX[0]);
		sscanf(factorYView[0]->Text(),"%f",&factorY[0]);
		sscanf(factorXView[1]->Text(),"%f",&factorX[1]);
		sscanf(factorYView[1]->Text(),"%f",&factorY[1]);
		sscanf(minimumMView->Text(),"%u",&minimumM);
/*		sscanf(speedleftView->Text(),"%u",&primdbltime);
		sscanf(speedrightView->Text(),"%u",&secdbltime);
		sscanf(speedmiddleView->Text(),"%u",&tertdbltime);
*/		minimumM=minimumM*minimumM;


    	lseek(handle,OFFSETFACTORX0,0);
		write(handle,&factorX[0],sizeof(float));

    	lseek(handle,OFFSETFACTORY0,0);
		write(handle,&factorY[0],sizeof(float));

    	lseek(handle,OFFSETFACTORX1,0);
		write(handle,&factorX[1],sizeof(float));

    	lseek(handle,OFFSETFACTORY1,0);
		write(handle,&factorY[1],sizeof(float));

		lseek(handle,OFFSETMINIMUM,0);
		write(handle,&minimumM,sizeof(int));

/*		primdbltime*=1000;
		lseek(handle,OFFSETTIMEPRIMARY,0);
		write(handle,&primdbltime,sizeof(int));

		secdbltime*=1000;
		lseek(handle,OFFSETTIMESECONDARY,0);
		write(handle,&secdbltime,sizeof(int));

		tertdbltime*=1000;
		lseek(handle,OFFSETTIMETERTIARY,0);
		write(handle,&tertdbltime,sizeof(int));
*/
	    strcpy(command[commandidx],eventcommand->Text());
    	lseek(handle,OFFSETCMD0,0);
		write(handle,command,255*9);
	    
//*/			
			close(handle);
			if (restartinputserver->Value()==B_CONTROL_ON) system("/system/servers/input_server -q");

		case MSG_CANCEL:
			QuitRequested();
		break;

		default:
			BWindow::MessageReceived(message);
	}
}

int main(int argc, char*argv[])
{	
	int i;
	char str[255];
	bool done=false;
	int lines=4,help;
	
	//randseed=real_time_clock();
	
//	strcpy(path,argv[0]);
//	i=strlen(path)-1;
//	while ((i>0) &&  (path[i]!='/')) i--;
//	path[i+1]=0;
	
	
	
	
//	printf("ProCalc V1.0 (-? for help)\n");

	for (i=1;i<argc;i++)
	{
		strcpy(str,argv[i]);
	 	str[2]=0;
		/*if (strcmp(str,"-c")==0)
		{
			sprintf(filename,argv[i]+2);
			printf("Using config-file : %s\n",filename);
			filehandle=open(filename,O_RDWR);
			if (filehandle==-1)
			{	
				filehandle=creat(filename,0x333);
				close(filehandle);
				filehandle=open(filename,O_RDWR);
				if (filehandle!=-1)
				{
					donewithfilename=true;
					close(filehandle);
				}
				
			}
			else donewithfilename=true;
			if (!donewithfilename)
				printf("Sorry. Problems openning the config-file %s\n",filename);
		}*/
	/*	if (strcmp(str,"-n")==0)
		{
			WINDOWFLOAT=B_NORMAL_WINDOW_FEEL;
		}
		if (strcmp(str,"-s")==0)
		{
			sprintf(str,argv[i]+2);
			help=atoi(str);
			if (help<=MAXLINES) lines=help;
			
		}
		if (strcmp(str,"-l")==0)
		{
			sprintf(str,argv[i]+2);
			help=atoi(str);
			LINEHEIGHT=20+10*help;
			
		}*/
		//printf("%d %s\n",i,argv[i]);	 	
	}

/*	if ((argc>1) && ((strcmp("-?",argv[1])==0) || (strcmp("-h",argv[1])==0) || (strcmp("--help",argv[1])==0) ))
	{
		printf("   -?        This Help-Message\n");
		printf("   -lXXX     Opens ProCalc with XXX lines per slot\n");
		printf("   -n        non-floating Window Feel\n");
		printf("   -sXXX     Opens ProCalc with XXX slots of Formula-Fields\n");
		done=true;
	}
/*
	if ((!donewithfilename) && (!done))
	{
		sprintf(filename,"%sProCalc.config",path);
		filehandle=open(filename,O_RDWR);
		if (filehandle==-1)
		{	
			sprintf(filename,"/boot/home/ProCalc.config");
			filehandle=open(filename,O_RDWR);
			if (filehandle==-1)
			{
				sprintf(filename,"%sProCalc.config",path);
				filehandle=creat(filename,0x0333);
				if (filehandle==-1)
				{	
					sprintf(filename,"/boot/home/ProCalc.config");
					filehandle=creat(filename,0x0333);
				}
			}
		}
		if (filehandle==-1)
		{
			printf("File Access Error. Storing of wrong and right letters disabled\n");
		}
		else
		{
			close(filehandle);
			filehandle=open(filename,O_RDWR);
			i=0;
			while (lseek(filehandle,0,SEEK_END)<MODIFIERS*CHARS*2*sizeof(int))
				write(filehandle,&i,sizeof(int));
		
			lseek(filehandle,0,SEEK_SET);
			for (i=0;i<MODIFIERS*CHARS;i++)
			{
				read(filehandle,&wrong[i],sizeof(int));
				read(filehandle,&right[i],sizeof(int));
			}
			
			close(filehandle);
			printf("storing data about letters in %s ...\n",filename);	
		}
	}//if donewithfilename==false
*/
	int myh=open("/boot/home/config/add-ons/input_server/filters/UniversalScroller",O_RDWR);

	if (myh!=-1)
	{
		close(myh);
		USPrefApplication	myApplication;
		myApplication.Run();
	}
	else
	system("alert \"Could not find the installed UniversalScroller \n(/boot/home/config/add-ons/input_server/filters/UniversalScroller)\"");
	return(0);
}
