/**********************************************************/
/*                UniversalScroller.cpp                   */
/**********************************************************/
/*                                                        */
/* This little util should help those, who used to work   */
/* with Logitech's MouseWare's UniversalScroll Option     */
/*                                                        */
/*--------------------------------------------------------*/
/*                                                        */
/* infos, updates, questions, ...                         */
/*    Troublemaker@altavista.net                          */
/*    http://come.to/Troublemaker                         */
/*                                                        */
/*--------------------------------------------------------*/
/*                                                        */
/*  Author   Date        What                             */
/*  Gisi   2000.09.13    V2.0 everything new ...          */
/*                                                        */
/**********************************************************/

#include <Message.h>
#include <View.h>
#include <List.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>





#include "TMsystem.h"
#include "button.h"
#include "Configuration.h"

#include "UniversalScroller.h"





UniversalScroller::UniversalScroller()
{
}

UniversalScroller::~UniversalScroller()
{
}

status_t UniversalScroller::InitCheck()
{
	return B_OK;
}

#define CREATE_MSG_OFFSET( KIND, OFFSET ) \
	msg=new BMessage( KIND ); \
	msg->AddInt64("when",system_time()+(OFFSET));

#define CREATE_MSG( KIND ) CREATE_MSG_OFFSET( KIND, 0 )
#define CREATE_OLD_MSG( KIND ) CREATE_MSG_OFFSET( KIND, -5 )

#define ENLIST_MSG() outList->AddItem(msg);

#define SEND_MOUSE_UP( BUTTONS ) \
	CREATE_MSG( B_MOUSE_UP ); \
	msg->AddPoint("where",mousePosition); \
	msg->AddInt32("modifiers",modifiers); \
	msg->AddInt32("buttons", BUTTONS ); \
	ENLIST_MSG();	

#define IS_MODIFIER_SET( MODIFIER )  ( ( modifiers & MODIFIER ) == MODIFIER )

filter_result UniversalScroller::Filter(BMessage *message, BList *outList)
{

 // Store the states of three used modifier keys.
 static bool isShiftKeyDown  =false;
 static bool isAltKeyDown    =false;
 static bool isControlKeyDown=false;

 // the last known mouse position
 static BPoint mousePosition;
 
 // the position, where the last mouse button went down
 static BPoint mouseButtonDownPosition;

 // msg holds the message that is going to be injected into outList
 BMessage *msg;

 // timestamp when the primary, secondare, tertiary mouse button was clicked last time
 static int64 mouseButtonDownLastTime[3]={0,0,0};
 
 static int64 click_count[3]={0,0,0};
 float deltax=0,deltay=0;
 filter_result res = B_DISPATCH_MESSAGE;
 int32 modifiers;
 int32 buttons;
 int cmdidx;
 int32 internal_buttons=0;
 static int32 old_internal_buttons=0;
 static int32 old_buttons=0;
 int i;
 int new_button_down;
 int new_clicks;
 int32 virtual_modifiers;
 
// char str[255];
// static int countadd=0;
 int32 buttonval[6]={B_PRIMARY_MOUSE_BUTTON,B_SECONDARY_MOUSE_BUTTON,B_TERTIARY_MOUSE_BUTTON,B_PRIMARY_MOUSE_BUTTON,B_SECONDARY_MOUSE_BUTTON,B_TERTIARY_MOUSE_BUTTON};

 //so now for the switching
 switch (message->what)
 {
	case B_MODIFIERS_CHANGED:
		
		message->FindInt32("modifiers",&modifiers);
		
		isShiftKeyDown  = IS_MODIFIER_SET( B_SHIFT_KEY   );
		isControlKeyDown= IS_MODIFIER_SET( B_CONTROL_KEY );
		isAltKeyDown    = IS_MODIFIER_SET( B_OPTION_KEY  );
		
		if ((isAltKeyDown) && (old_internal_buttons!=0))
		{
			old_internal_buttons=0;
			old_buttons=0;
			
			SEND_MOUSE_UP( old_internal_buttons );
		}
		break;
	
	case B_MOUSE_WHEEL_CHANGED:
		float hfloat;
	  	if ((!isAltKeyDown) && ((isShiftKeyDown) || (configuration.factorsforwheel)))
	  	{                               
	  		message->FindFloat("be:wheel_delta_x",&deltax);
	  		message->FindFloat("be:wheel_delta_y",&deltay);

			CREATE_MSG( B_MOUSE_WHEEL_CHANGED );
			if (isShiftKeyDown) {hfloat=deltax;deltax=deltay;deltay=hfloat;}
		  	if (configuration.factorsforwheel) {deltax*=configuration.factorX[isControlKeyDown?1:0];deltay*=configuration.factorY[isControlKeyDown?1:0];}
		  	msg->AddFloat("be:wheel_delta_x",deltax); //so swap x and y here
	  		msg->AddFloat("be:wheel_delta_y",deltay);
	  		ENLIST_MSG();
	  	}
		break;
		
	case B_MOUSE_DOWN:
		if (!isAltKeyDown)
		{
		  	message->FindPoint("where",&mouseButtonDownPosition);
		  	message->FindInt32("buttons",&buttons);
			message->FindInt32("modifiers",&modifiers);
			res=B_SKIP_MESSAGE;
		
			cmdidx=-1;
			if ((old_buttons==0) && (buttons==1)) cmdidx=0;
			if ((old_buttons==1) && (buttons==3)) cmdidx=1;
			if ((old_buttons==1) && (buttons==5)) cmdidx=2;
			if ((old_buttons==0) && (buttons==2)) cmdidx=3;
			if ((old_buttons==2) && (buttons==3)) cmdidx=4;
			if ((old_buttons==2) && (buttons==6)) cmdidx=5;
			if ((old_buttons==0) && (buttons==4)) cmdidx=6;
			if ((old_buttons==4) && (buttons==5)) cmdidx=7;
			if ((old_buttons==4) && (buttons==6)) cmdidx=8;

			if ((cmdidx!=-1) && (!configuration.swallowclick[cmdidx]))
			{
				int32 key_var=0;
				int32 rawchar=0;
				int8 numbytes=0,byte[3]={0,0,0};
				switch ( configuration.buttonDownCommand[cmdidx].kind )
				{

					case button:
					    new_button_down=configuration.buttonDownCommand[cmdidx].mouseButtonIndex;
					    new_clicks=configuration.buttonDownCommand[cmdidx].mouseButtonClicks;
					    
						internal_buttons=old_internal_buttons|buttonval[new_button_down];

						//zu langsam für doppelklick
						if ( system_time() - mouseButtonDownLastTime[new_button_down]
						     > configuration.doubleClickSpeed[new_button_down] )
						{
							click_count[new_button_down]=0;
						}
							
						mouseButtonDownLastTime[new_button_down]=system_time();
						
						while (new_clicks>0)
						{
							click_count[new_button_down]++;
							new_clicks--;
							CREATE_MSG( B_MOUSE_DOWN );
							msg->AddPoint("where",mouseButtonDownPosition);
							msg->AddInt32("modifiers",modifiers);
							msg->AddInt32("buttons",internal_buttons);
							msg->AddInt32("clicks",click_count[new_button_down]);
							ENLIST_MSG();
						
							if (new_clicks>0)
							{
								SEND_MOUSE_UP( old_internal_buttons );
							}	
						}
						res=B_DISPATCH_MESSAGE;
					
						old_internal_buttons=internal_buttons;

						break;
											
					case key:
						//format of option is: "KEY_SHIFT_OPTION_CONTROL_key_raw-char_byte_#bytes_byte0[_byte1[_byte2]]_bytesZ"
						char *bytes;
						i=strlen(KEY);
						
						
						virtual_modifiers=0;
						if (strncmp(configuration.buttonDownCommand[cmdidx].command+i,SHIFT,strlen(SHIFT))==0) {virtual_modifiers|=B_SHIFT_KEY;i+=strlen(SHIFT);}
						if (strncmp(configuration.buttonDownCommand[cmdidx].command+i,OPTION,strlen(OPTION))==0) {virtual_modifiers|=B_OPTION_KEY;i+=strlen(OPTION);}
						if (strncmp(configuration.buttonDownCommand[cmdidx].command+i,CONTROL,strlen(CONTROL))==0) {virtual_modifiers|=B_CONTROL_KEY;i+=strlen(CONTROL);}
	
						bytes=((char *)(configuration.buttonDownCommand[cmdidx].command))+i+1;
						key_var=atoi(bytes); 			bytes=strstr(bytes,"_")+1;
						rawchar=atoi(bytes);		bytes=strstr(bytes,"_")+1;
						numbytes=atoi(bytes);		bytes=strstr(bytes,"_")+1;
						for (i=0;i<numbytes;i++)
							byte[i]=atoi(bytes);	bytes=strstr(bytes,"_")+1;
	
						CREATE_MSG( B_MODIFIERS_CHANGED );
						msg->AddInt32("modifiers",virtual_modifiers);
						msg->AddInt32("be:old_modifiers",modifiers);
						ENLIST_MSG();	
	
						CREATE_MSG( B_KEY_DOWN );
						msg->AddInt32("modifiers",virtual_modifiers);
						msg->AddInt32("key",key_var);
						msg->AddInt32("raw_char",rawchar);
						for (i=0;i<numbytes;i++)
							msg->AddInt8("byte",byte[i]);
						msg->AddString("bytes",bytes);
						msg->AddInt8("states",0);
						ENLIST_MSG();	
	
						CREATE_MSG( B_KEY_UP );
						msg->AddInt32("modifiers",virtual_modifiers);
						msg->AddInt32("key",key_var);
						msg->AddInt32("raw_char",rawchar);
						for (i=0;i<numbytes;i++)
							msg->AddInt8("byte",byte[i]);
						msg->AddString("bytes",bytes);
						msg->AddInt8("states",0);
						ENLIST_MSG();	
	
						CREATE_MSG( B_MODIFIERS_CHANGED );
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("be:old_modifiers",virtual_modifiers);
						ENLIST_MSG();	
	
						res=B_DISPATCH_MESSAGE;
	
						break;
						
					case cut:
						CREATE_OLD_MSG( 'CCUT' );
						ENLIST_MSG();	
						res=B_DISPATCH_MESSAGE;
						
						break;	

					case copy:
						CREATE_OLD_MSG( 'COPY' );
						ENLIST_MSG();	
						res=B_DISPATCH_MESSAGE;
				
						break;

					case paste:
						CREATE_OLD_MSG( 'PSTE' );
						ENLIST_MSG();	
						res=B_DISPATCH_MESSAGE;
				
						break;
						
					case executable:
						TMsystem(configuration.buttonDownCommand[cmdidx].command);
						
						break;		
				}
				 
				
			}
			old_buttons=buttons;
		}
  	break;	

	case B_MOUSE_UP:
		int i;
		char str[31];
	
		if (!isAltKeyDown)
		{
		  	message->FindPoint("where",&mousePosition);
		  	message->FindInt32("buttons",&buttons);
			message->FindInt32("modifiers",&modifiers);
	
			res=B_SKIP_MESSAGE;
			
			if (old_internal_buttons!=0)
			{
				if (buttons==0)
				{
					SEND_MOUSE_UP( 0 );
					res=B_DISPATCH_MESSAGE;
					old_internal_buttons=0;
				}
				else
				{
					internal_buttons=old_internal_buttons;
					for (i=0;i<6;i++)
						if ((old_internal_buttons&buttonval[i])==buttonval[i])
						{
							if (i==0) strcpy(str,LEFT);
							if (i==1) strcpy(str,MIDDLE);
							if (i==2) strcpy(str,RIGHT);
							if (i==3) strcpy(str,LEFTDBL);
							if (i==4) strcpy(str,MIDDLEDBL);
							if (i==5) strcpy(str,RIGHTDBL);
							if
							(!( 
							((((buttons&B_PRIMARY_MOUSE_BUTTON)==B_PRIMARY_MOUSE_BUTTON) && (strcasecmp(str,configuration.buttonDownCommand[0].command)==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.buttonDownCommand[1].command)==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.buttonDownCommand[2].command)==0)))

							||
							((((buttons&B_SECONDARY_MOUSE_BUTTON)==B_SECONDARY_MOUSE_BUTTON) && (strcasecmp(str,configuration.buttonDownCommand[3].command)==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.buttonDownCommand[4].command)==0)))
							||
							((((buttons&(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.buttonDownCommand[5].command)==0)))
	
							||
							((((buttons&B_TERTIARY_MOUSE_BUTTON)==B_TERTIARY_MOUSE_BUTTON) && (strcasecmp(str,configuration.buttonDownCommand[6].command)==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.buttonDownCommand[7].command)==0)))
							||
							((((buttons&(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.buttonDownCommand[8].command)==0)))
							))
							{
							internal_buttons=(internal_buttons-buttonval[i]);
							}
						
						}
					//for und if hier zuende
					if (internal_buttons!=old_internal_buttons)
					{
						SEND_MOUSE_UP( old_internal_buttons );
						res=B_DISPATCH_MESSAGE;
						old_internal_buttons=internal_buttons;
					}
				}
			}
			old_buttons=buttons;
		}
  	break;	
	
	case B_MOUSE_MOVED:
		if (!isAltKeyDown)
		{
		  	message->FindInt32("buttons",&buttons);
			message->FindInt32("modifiers",&modifiers);
			if (configuration.scrollmousedown[buttons])
			{	
				message->FindPoint("where",&mousePosition);
				deltax=configuration.factorX[isControlKeyDown?1:0]*(mousePosition.x-mouseButtonDownPosition.x);
				deltay=configuration.factorY[isControlKeyDown?1:0]*(mousePosition.y-mouseButtonDownPosition.y);

				if ((deltax*deltax>configuration.minScroll) || (deltay*deltay>configuration.minScroll))
				{	
					if (old_internal_buttons!=0)
					{
						SEND_MOUSE_UP( 0 );					
						old_internal_buttons=0;
					
					}
					
					CREATE_MSG( B_MOUSE_WHEEL_CHANGED );
				  	if (isShiftKeyDown) {hfloat=deltax;deltax=deltay;deltay=hfloat;}
		  			msg->AddFloat("be:wheel_delta_x",deltax); //so swap x and y here
		  			msg->AddFloat("be:wheel_delta_y",deltay);
			  		ENLIST_MSG();
				}
	
			}
		}
  	break;	
	
 }
 return (res);
}
