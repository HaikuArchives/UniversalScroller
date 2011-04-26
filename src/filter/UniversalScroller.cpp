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


filter_result UniversalScroller::Filter(BMessage *message, BList *outList)
{

 // Store the states of three used modifier keys.
 static bool isShiftKeyDown=false;
 static bool isAltKeyDown=false;
 static bool isControlKeyDown=false;

 BMessage *msg;

 BPoint mousepoint;
 static BPoint mousedown;
 static int64 last_click_time[3]={0,0,0};
 static int64 click_count[3]={0,0,0};
 float deltax=0,deltay=0;
 filter_result res = B_DISPATCH_MESSAGE;
 int32 modifiers;
 int32 buttons;
 int cmdidx;
 int32 internal_buttons=0;
 static int32 old_internal_buttons=0;
 static int32 old_buttons=0;
 bool done;
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
		isShiftKeyDown=((modifiers&B_SHIFT_KEY)==B_SHIFT_KEY);
		isControlKeyDown=((modifiers&B_CONTROL_KEY)==B_CONTROL_KEY);
		isAltKeyDown=((modifiers&B_OPTION_KEY)==B_OPTION_KEY);
		if ((isAltKeyDown) && (old_internal_buttons!=0))
		{
			old_internal_buttons=0;
			old_buttons=0;
			
			CREATE_MSG( B_MOUSE_UP );
			msg->AddPoint("where",mousepoint);
			msg->AddInt32("modifiers",modifiers);
			msg->AddInt32("buttons",old_internal_buttons);
			ENLIST_MSG();
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
		  	message->FindPoint("where",&mousedown);
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
				done=false;
				new_button_down=-1;  
				if (strncasecmp(configuration.cmd[cmdidx],LEFT,strlen(LEFT))==0) {done=true;new_button_down=0;if (strlen(configuration.cmd[cmdidx])==strlen(LEFT))new_clicks=1;else new_clicks=atoi(configuration.cmd[cmdidx]+strlen(LEFT));}
				if (strncasecmp(configuration.cmd[cmdidx],RIGHT,strlen(RIGHT))==0) {done=true;new_button_down=1;if (strlen(configuration.cmd[cmdidx])==strlen(RIGHT))new_clicks=1;else new_clicks=atoi(configuration.cmd[cmdidx]+strlen(RIGHT));}
				if (strncasecmp(configuration.cmd[cmdidx],MIDDLE,strlen(MIDDLE))==0) {done=true;new_button_down=2;if (strlen(configuration.cmd[cmdidx])==strlen(MIDDLE))new_clicks=1;else new_clicks=atoi(configuration.cmd[cmdidx]+strlen(MIDDLE));}
				if (strncasecmp(configuration.cmd[cmdidx],LEFTDBL,strlen(LEFTDBL))==0) {done=true;new_button_down=0;new_clicks=2;}
				if (strncasecmp(configuration.cmd[cmdidx],RIGHTDBL,strlen(RIGHTDBL))==0) {done=true;new_button_down=1;new_clicks=2;}
				if (strncasecmp(configuration.cmd[cmdidx],MIDDLEDBL,strlen(MIDDLEDBL))==0) {done=true;new_button_down=2;new_clicks=2;}
				if (new_button_down!=-1)
				{
					internal_buttons=old_internal_buttons|buttonval[new_button_down];

					//zu langsam für doppelklick
					if (system_time()-last_click_time[new_button_down]>configuration.doubleClickSpeed[new_button_down])
						click_count[new_button_down]=0;
						
					last_click_time[new_button_down]=system_time();
					
					while (new_clicks>0)
					{
						click_count[new_button_down]++;
						new_clicks--;
						CREATE_MSG( B_MOUSE_DOWN );
						msg->AddPoint("where",mousedown);
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("buttons",internal_buttons);
						msg->AddInt32("clicks",click_count[new_button_down]);
						ENLIST_MSG();
					
						if (new_clicks>0)
						{
							CREATE_MSG( B_MOUSE_UP );
							msg->AddPoint("where",mousepoint);
							msg->AddInt32("modifiers",modifiers);
							msg->AddInt32("buttons",old_internal_buttons);
							ENLIST_MSG();	
						}	
					}
					res=B_DISPATCH_MESSAGE;
				
					old_internal_buttons=internal_buttons;
				}
				
				if (strcasecmp(configuration.cmd[cmdidx],CUT)==0)
				{	CREATE_OLD_MSG( 'CCUT' );
					ENLIST_MSG();	
					done=true;
					res=B_DISPATCH_MESSAGE;
				}

				if (strcasecmp(configuration.cmd[cmdidx],COPY)==0)
				{	CREATE_OLD_MSG( 'COPY' );
					ENLIST_MSG();	
					done=true;
					res=B_DISPATCH_MESSAGE;
				}

				if (strcasecmp(configuration.cmd[cmdidx],PASTE)==0)
				{	CREATE_OLD_MSG( 'PSTE' );
					ENLIST_MSG();	
					done=true;
					res=B_DISPATCH_MESSAGE;
				}
				
				if (strncasecmp(configuration.cmd[cmdidx],KEY,strlen(KEY))==0)
				{
//"KEY_SHIFT_OPTION_CONTROL_key_raw-char_byte_#bytes_byte0[_byte1[_byte2]]_bytesZ"
					int32 key=0;
					int32 rawchar=0;
					int8 numbytes=0,byte[3]={0,0,0};
					char *bytes;
					i=strlen(KEY);
					
					
					virtual_modifiers=0;
					if (strncmp(configuration.cmd[cmdidx]+i,SHIFT,strlen(SHIFT))==0) {virtual_modifiers|=B_SHIFT_KEY;i+=strlen(SHIFT);}
					if (strncmp(configuration.cmd[cmdidx]+i,OPTION,strlen(OPTION))==0) {virtual_modifiers|=B_OPTION_KEY;i+=strlen(OPTION);}
					if (strncmp(configuration.cmd[cmdidx]+i,CONTROL,strlen(CONTROL))==0) {virtual_modifiers|=B_CONTROL_KEY;i+=strlen(CONTROL);}

					bytes=configuration.cmd[cmdidx]+i+1;
					key=atoi(bytes); 			bytes=strstr(bytes,"_")+1;
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
					msg->AddInt32("key",key);
					msg->AddInt32("raw_char",rawchar);
					for (i=0;i<numbytes;i++)
						msg->AddInt8("byte",byte[i]);
					msg->AddString("bytes",bytes);
					msg->AddInt8("states",0);
					ENLIST_MSG();	

					CREATE_MSG( B_KEY_UP );
					msg->AddInt32("modifiers",virtual_modifiers);
					msg->AddInt32("key",key);
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

					done=true;
				}
				if (!done)
				{
					TMsystem(configuration.cmd[cmdidx]);
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
		  	message->FindPoint("where",&mousepoint);
		  	message->FindInt32("buttons",&buttons);
			message->FindInt32("modifiers",&modifiers);
	
			res=B_SKIP_MESSAGE;
			
			if (old_internal_buttons!=0)
			{
				if (buttons==0)
				{
					CREATE_MSG( B_MOUSE_UP );
					msg->AddPoint("where",mousepoint);
					msg->AddInt32("modifiers",modifiers);
					msg->AddInt32("buttons",0);
					ENLIST_MSG();	
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
							((((buttons&B_PRIMARY_MOUSE_BUTTON)==B_PRIMARY_MOUSE_BUTTON) && (strcasecmp(str,configuration.cmd[0])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.cmd[1])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.cmd[2])==0)))

							||
							((((buttons&B_SECONDARY_MOUSE_BUTTON)==B_SECONDARY_MOUSE_BUTTON) && (strcasecmp(str,configuration.cmd[3])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.cmd[4])==0)))
							||
							((((buttons&(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.cmd[5])==0)))
	
							||
							((((buttons&B_TERTIARY_MOUSE_BUTTON)==B_TERTIARY_MOUSE_BUTTON) && (strcasecmp(str,configuration.cmd[6])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.cmd[7])==0)))
							||
							((((buttons&(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,configuration.cmd[8])==0)))
							))
							{
							internal_buttons=(internal_buttons-buttonval[i]);
							}
						
						}
					//for und if hier zuende
					if (internal_buttons!=old_internal_buttons)
					{
						CREATE_MSG( B_MOUSE_UP );
						msg->AddPoint("where",mousepoint);
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("buttons",internal_buttons);
						ENLIST_MSG();	
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
				message->FindPoint("where",&mousepoint);
				deltax=configuration.factorX[isControlKeyDown?1:0]*(mousepoint.x-mousedown.x);
				deltay=configuration.factorY[isControlKeyDown?1:0]*(mousepoint.y-mousedown.y);

				if ((deltax*deltax>configuration.minScroll) || (deltay*deltay>configuration.minScroll))
				{	
					if (old_internal_buttons!=0)
					{
						CREATE_MSG( B_MOUSE_UP );
						msg->AddPoint("where",mousepoint);
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("buttons",0);
						ENLIST_MSG();	
					
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
