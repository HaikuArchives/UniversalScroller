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

#include <be/support/TypeConstants.h>

#include <add-ons/input_server/InputServerFilter.h>

#include "../includes/TMsystem.h"

#include "button.h"

extern "C" _EXPORT BInputServerFilter* instantiate_input_filter();

class WheelEnhanceFilter : public BInputServerFilter 
{
	private:
	public:
		WheelEnhanceFilter();
		virtual ~WheelEnhanceFilter();
		virtual	filter_result Filter(BMessage *message, BList *outList);
    	virtual status_t InitCheck(); 


};

BInputServerFilter* instantiate_input_filter()
{
	return (new WheelEnhanceFilter());
}


WheelEnhanceFilter::WheelEnhanceFilter()
{
}

WheelEnhanceFilter::~WheelEnhanceFilter()
{
}

status_t WheelEnhanceFilter::InitCheck()
{
	return B_OK;
}

filter_result WheelEnhanceFilter::Filter(BMessage *message, BList *outList)
{


 static float factorX[2]={0.1,1.0},factorY[2]={0.1,1.0};
 static int minscroll=0;
 static int double_click_speed[3]={250000,250000,250000};
 static char cmd[9][255]={LEFT,"beep","beep",RIGHT,"beep","beep",MIDDLE,COPY,PASTE};
 static bool factorsforwheel=true;
 static bool scrollmousedown[8]={false,false,false,false,true,true,true,true};
 static bool swallowclick[8]={false,false,false,false,true,true,true,true};



 static bool shiftdown=false;
 static bool altdown=false;
 static bool controldown=false;

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
		shiftdown=((modifiers&B_SHIFT_KEY)==B_SHIFT_KEY);
		controldown=((modifiers&B_CONTROL_KEY)==B_CONTROL_KEY);
		altdown=((modifiers&B_OPTION_KEY)==B_OPTION_KEY);
		if ((altdown) && (old_internal_buttons!=0))
		{
			old_internal_buttons=0;
			old_buttons=0;
			msg=new BMessage(B_MOUSE_UP);
			msg->AddInt64("when",system_time());
			msg->AddPoint("where",mousepoint);
			msg->AddInt32("modifiers",modifiers);
			msg->AddInt32("buttons",old_internal_buttons);
			outList->AddItem(msg);	
		}
		break;
	
	case B_MOUSE_WHEEL_CHANGED:
		float hfloat;
	  	if ((!altdown) && ((shiftdown) || (factorsforwheel)))
	  	{                               
	  		message->FindFloat("be:wheel_delta_x",&deltax);
	  		message->FindFloat("be:wheel_delta_y",&deltay);

			msg=new BMessage(B_MOUSE_WHEEL_CHANGED);
			msg->AddInt64("when",system_time());
		  	if (shiftdown) {hfloat=deltax;deltax=deltay;deltay=hfloat;}
		  	if (factorsforwheel) {deltax*=factorX[controldown?1:0];deltay*=factorY[controldown?1:0];}
		  	msg->AddFloat("be:wheel_delta_x",deltax); //so swap x and y here
	  		msg->AddFloat("be:wheel_delta_y",deltay);
	  		outList->AddItem(msg);
	  	}
		break;
		
	case B_MOUSE_DOWN:
		if (!altdown)
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

			if ((cmdidx!=-1) && (!swallowclick[cmdidx]))
			{
				done=false;
				new_button_down=-1;  
				if (strncasecmp(cmd[cmdidx],LEFT,strlen(LEFT))==0) {done=true;new_button_down=0;if (strlen(cmd[cmdidx])==strlen(LEFT))new_clicks=1;else new_clicks=atoi(cmd[cmdidx]+strlen(LEFT));}
				if (strncasecmp(cmd[cmdidx],RIGHT,strlen(RIGHT))==0) {done=true;new_button_down=1;if (strlen(cmd[cmdidx])==strlen(RIGHT))new_clicks=1;else new_clicks=atoi(cmd[cmdidx]+strlen(RIGHT));}
				if (strncasecmp(cmd[cmdidx],MIDDLE,strlen(MIDDLE))==0) {done=true;new_button_down=2;if (strlen(cmd[cmdidx])==strlen(MIDDLE))new_clicks=1;else new_clicks=atoi(cmd[cmdidx]+strlen(MIDDLE));}
				if (strncasecmp(cmd[cmdidx],LEFTDBL,strlen(LEFTDBL))==0) {done=true;new_button_down=0;new_clicks=2;}
				if (strncasecmp(cmd[cmdidx],RIGHTDBL,strlen(RIGHTDBL))==0) {done=true;new_button_down=1;new_clicks=2;}
				if (strncasecmp(cmd[cmdidx],MIDDLEDBL,strlen(MIDDLEDBL))==0) {done=true;new_button_down=2;new_clicks=2;}
				if (new_button_down!=-1)
				{
					internal_buttons=old_internal_buttons|buttonval[new_button_down];

					//zu langsam für doppelklick
					if (system_time()-last_click_time[new_button_down]>double_click_speed[new_button_down])
						click_count[new_button_down]=0;
						
					last_click_time[new_button_down]=system_time();
					
					while (new_clicks>0)
					{
						click_count[new_button_down]++;
						new_clicks--;
						msg=new BMessage(B_MOUSE_DOWN);
						msg->AddInt64("when",system_time());
						msg->AddPoint("where",mousedown);
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("buttons",internal_buttons);
						msg->AddInt32("clicks",click_count[new_button_down]);
						outList->AddItem(msg);
					
						if (new_clicks>0)
						{
							msg=new BMessage(B_MOUSE_UP);
							msg->AddInt64("when",system_time());
							msg->AddPoint("where",mousepoint);
							msg->AddInt32("modifiers",modifiers);
							msg->AddInt32("buttons",old_internal_buttons);
							outList->AddItem(msg);	
						}	
					}
					res=B_DISPATCH_MESSAGE;
				
					old_internal_buttons=internal_buttons;
				}
				
				if (strcasecmp(cmd[cmdidx],CUT)==0)
				{	msg=new BMessage('CCUT');
					msg->AddInt64("when",system_time()-5);
					outList->AddItem(msg);	
					done=true;
					res=B_DISPATCH_MESSAGE;
				}

				if (strcasecmp(cmd[cmdidx],COPY)==0)
				{	msg=new BMessage('COPY');
					msg->AddInt64("when",system_time()-5);
					outList->AddItem(msg);	
					done=true;
					res=B_DISPATCH_MESSAGE;
				}

				if (strcasecmp(cmd[cmdidx],PASTE)==0)
				{	msg=new BMessage('PSTE');
					msg->AddInt64("when",system_time()-5);
					outList->AddItem(msg);	
					done=true;
					res=B_DISPATCH_MESSAGE;
				}
				
				if (strncasecmp(cmd[cmdidx],KEY,strlen(KEY))==0)
				{
//"KEY_SHIFT_OPTION_CONTROL_key_raw-char_byte_#bytes_byte0[_byte1[_byte2]]_bytesZ"
					int32 key=0;
					int32 rawchar=0;
					int8 numbytes=0,byte[3]={0,0,0};
					char *bytes;
					i=strlen(KEY);
					
					
					virtual_modifiers=0;
					if (strncmp(cmd[cmdidx]+i,SHIFT,strlen(SHIFT))==0) {virtual_modifiers|=B_SHIFT_KEY;i+=strlen(SHIFT);}
					if (strncmp(cmd[cmdidx]+i,OPTION,strlen(OPTION))==0) {virtual_modifiers|=B_OPTION_KEY;i+=strlen(OPTION);}
					if (strncmp(cmd[cmdidx]+i,CONTROL,strlen(CONTROL))==0) {virtual_modifiers|=B_CONTROL_KEY;i+=strlen(CONTROL);}

					bytes=cmd[cmdidx]+i+1;
					key=atoi(bytes); 			bytes=strstr(bytes,"_")+1;
					rawchar=atoi(bytes);		bytes=strstr(bytes,"_")+1;
					numbytes=atoi(bytes);		bytes=strstr(bytes,"_")+1;
					for (i=0;i<numbytes;i++)
						byte[i]=atoi(bytes);	bytes=strstr(bytes,"_")+1;

					msg=new BMessage(B_MODIFIERS_CHANGED);
					msg->AddInt64("when",system_time());
					msg->AddInt32("modifiers",virtual_modifiers);
					msg->AddInt32("be:old_modifiers",modifiers);
					outList->AddItem(msg);	

					msg=new BMessage(B_KEY_DOWN);
					msg->AddInt64("when",system_time());
					msg->AddInt32("modifiers",virtual_modifiers);
					msg->AddInt32("key",key);
					msg->AddInt32("raw_char",rawchar);
					for (i=0;i<numbytes;i++)
						msg->AddInt8("byte",byte[i]);
					msg->AddString("bytes",bytes);
					msg->AddInt8("states",0);
					outList->AddItem(msg);	

					msg=new BMessage(B_KEY_UP);
					msg->AddInt64("when",system_time());
					msg->AddInt32("modifiers",virtual_modifiers);
					msg->AddInt32("key",key);
					msg->AddInt32("raw_char",rawchar);
					for (i=0;i<numbytes;i++)
						msg->AddInt8("byte",byte[i]);
					msg->AddString("bytes",bytes);
					msg->AddInt8("states",0);
					outList->AddItem(msg);	

					msg=new BMessage(B_MODIFIERS_CHANGED);
					msg->AddInt64("when",system_time());
					msg->AddInt32("modifiers",modifiers);
					msg->AddInt32("be:old_modifiers",virtual_modifiers);
					outList->AddItem(msg);	

					res=B_DISPATCH_MESSAGE;

					done=true;
				}
				if (!done)
				{
					TMsystem(cmd[cmdidx]);
				}
			}
			old_buttons=buttons;
		}
  	break;	

	case B_MOUSE_UP:
		int i;
		char str[31];
	
		if (!altdown)
		{
		  	message->FindPoint("where",&mousepoint);
		  	message->FindInt32("buttons",&buttons);
			message->FindInt32("modifiers",&modifiers);
	
			res=B_SKIP_MESSAGE;
			
			if (old_internal_buttons!=0)
			{
				if (buttons==0)
				{
					msg=new BMessage(B_MOUSE_UP);
					msg->AddInt64("when",system_time());
					msg->AddPoint("where",mousepoint);
					msg->AddInt32("modifiers",modifiers);
					msg->AddInt32("buttons",0);
					outList->AddItem(msg);	
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
							((((buttons&B_PRIMARY_MOUSE_BUTTON)==B_PRIMARY_MOUSE_BUTTON) && (strcasecmp(str,cmd[0])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON)) && (strcasecmp(str,cmd[1])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,cmd[2])==0)))

							||
							((((buttons&B_SECONDARY_MOUSE_BUTTON)==B_SECONDARY_MOUSE_BUTTON) && (strcasecmp(str,cmd[3])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_SECONDARY_MOUSE_BUTTON)) && (strcasecmp(str,cmd[4])==0)))
							||
							((((buttons&(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,cmd[5])==0)))
	
							||
							((((buttons&B_TERTIARY_MOUSE_BUTTON)==B_TERTIARY_MOUSE_BUTTON) && (strcasecmp(str,cmd[6])==0)))
							||
							((((buttons&(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_PRIMARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,cmd[7])==0)))
							||
							((((buttons&(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON))==(B_SECONDARY_MOUSE_BUTTON|B_TERTIARY_MOUSE_BUTTON)) && (strcasecmp(str,cmd[8])==0)))
							))
							{
							internal_buttons=(internal_buttons-buttonval[i]);
							}
						
						}
					//for und if hier zuende
					if (internal_buttons!=old_internal_buttons)
					{
						msg=new BMessage(B_MOUSE_UP);
						msg->AddInt64("when",system_time());
						msg->AddPoint("where",mousepoint);
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("buttons",internal_buttons);
						outList->AddItem(msg);	
						res=B_DISPATCH_MESSAGE;
						old_internal_buttons=internal_buttons;
					}
				}
			}
			old_buttons=buttons;
		}
  	break;	
	
	case B_MOUSE_MOVED:
		if (!altdown)
		{
		  	message->FindInt32("buttons",&buttons);
			message->FindInt32("modifiers",&modifiers);
			if (scrollmousedown[buttons])
			{	
				message->FindPoint("where",&mousepoint);
				deltax=factorX[controldown?1:0]*(mousepoint.x-mousedown.x);
				deltay=factorY[controldown?1:0]*(mousepoint.y-mousedown.y);

				if ((deltax*deltax>minscroll) || (deltay*deltay>minscroll))
				{	
					if (old_internal_buttons!=0)
					{
						msg=new BMessage(B_MOUSE_UP);
						msg->AddInt64("when",system_time());
						msg->AddPoint("where",mousepoint);
						msg->AddInt32("modifiers",modifiers);
						msg->AddInt32("buttons",0);
						outList->AddItem(msg);	
					
						old_internal_buttons=0;
					
					}
					
					msg=new BMessage(B_MOUSE_WHEEL_CHANGED);
					msg->AddInt64("when",system_time());
				  	if (shiftdown) {hfloat=deltax;deltax=deltay;deltay=hfloat;}
		  			msg->AddFloat("be:wheel_delta_x",deltax); //so swap x and y here
		  			msg->AddFloat("be:wheel_delta_y",deltay);
			  		outList->AddItem(msg);
				}
	
			}
		}
  	break;	
	
 }
 return (res);
}
