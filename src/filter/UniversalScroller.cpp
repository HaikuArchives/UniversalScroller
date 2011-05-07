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
	msg->AddPoint("where",physicalPosition_s); \
	msg->AddInt32("modifiers",physicalModifiers); \
	msg->AddInt32("buttons", BUTTONS ); \
	ENLIST_MSG();	

#define IS_MODIFIER_SET( MODIFIER )  ( ( physicalModifiers & MODIFIER ) == MODIFIER )

#define SET_VIRTUAL_MODIFIER( MODIFIER, MODIFIER_MASK ) \
	if ( strncmp( command+offset, MODIFIER, strlen( MODIFIER ) ) == 0 ) \
	{                                                                   \
		virtual_modifiers |= MODIFIER_MASK;                             \
		offset += strlen( MODIFIER );                                   \
	}

#define NIBBLE_INT( VAR ) \
	{ 										\
		VAR = atoi(toProcess);              \
 		toProcess=strstr(toProcess,"_")+1;  \
	}
	
#define	REBUILD_VIRTUAL_BUTTONS_DOWN_FROM_PRESSED( MASK ) \
	if ( ( physicalButtonsDown & MASK ) == MASK )                          \
	{                                                                      \
		cmdidx=Configuration::getButtonDownIndex( 0, MASK );               \
		if ( configuration.buttonDownCommand[cmdidx].kind == button )      \
		{                                                                  \
			int commandButtonDownMask =                                    \
				ButtonDownCommand::mouseButtonIndexToMask(                 \
				  configuration.buttonDownCommand[cmdidx].mouseButtonIndex \
				);                                                         \
 			if ( ( virtualButtonsDown_s & commandButtonDownMask ) ==       \
 				commandButtonDownMask )                                    \
 			{                                                              \
 				virtualButtonsDown |= commandButtonDownMask;               \
 			}                                                              \
		}	                                                               \
	}	                                                                   
	

void simulate_keypress( const char *command, int32 physicalModifiers, BList *outList )
{
	//format of commans is: "KEY_SHIFT_OPTION_CONTROL_key_raw-char_byte_#bytes_byte0[_byte1[_byte2]]_bytesZ"

	// msg holds the message that is going to be injected into outList
	BMessage *msg;

	int offset = strlen(KEY);
	int i;

	// the modifiers used for the injected key presses
	int32 virtual_modifiers = 0;
	SET_VIRTUAL_MODIFIER( SHIFT  , B_SHIFT_KEY   );
	SET_VIRTUAL_MODIFIER( OPTION , B_OPTION_KEY  );
	SET_VIRTUAL_MODIFIER( CONTROL, B_CONTROL_KEY );
	
	const char *toProcess = command + offset + 1;

	int32 key_var=0;
	NIBBLE_INT( key_var );
	int32 raw_char;
	NIBBLE_INT( raw_char );

	int8 numbytes=0;
	NIBBLE_INT( numbytes );

	int8 byte[3]={0,0,0};
	for (i=0;i<numbytes;i++)
	{
		NIBBLE_INT( byte[i] );
	}
	
	const char *bytes=toProcess;
	
	CREATE_MSG( B_MODIFIERS_CHANGED );
	msg->AddInt32("modifiers",virtual_modifiers);
	msg->AddInt32("be:old_modifiers",physicalModifiers);
	ENLIST_MSG();	
	
	CREATE_MSG( B_KEY_DOWN );
	msg->AddInt32("modifiers",virtual_modifiers);
	msg->AddInt32("key",key_var);
	msg->AddInt32("raw_char",raw_char);
	for (i=0;i<numbytes;i++)
		msg->AddInt8("byte",byte[i]);
	msg->AddString("bytes",bytes);
	msg->AddInt8("states",0);
	ENLIST_MSG();	
	
	CREATE_MSG( B_KEY_UP );
	msg->AddInt32("modifiers",virtual_modifiers);
	msg->AddInt32("key",key_var);
	msg->AddInt32("raw_char",raw_char);
	for (i=0;i<numbytes;i++)
		msg->AddInt8("byte",byte[i]);
	msg->AddString("bytes",bytes);
	msg->AddInt8("states",0);
	ENLIST_MSG();	
	
	CREATE_MSG( B_MODIFIERS_CHANGED );
	msg->AddInt32("modifiers",physicalModifiers);
	msg->AddInt32("be:old_modifiers",virtual_modifiers);
	ENLIST_MSG();	
	
}

/*
  physical -> represents something that appears to the a physical property instead of an augmented, virtual one.
  virtual  -> represents something that is only augmented downstream.
  _s       -> /s/tate. Indicates variables representing the internal state, being conserved between messages
*/

filter_result UniversalScroller::Filter(BMessage *message, BList *outList)
{
	/* =============================================================
	Static variables for storing the keyboard/mouse state between messages
    ============================================================= */
 
	// States of the relevant modifier keys
	static bool physicalIsShiftKeyDown_s  =false;
	static bool physicalIsAltKeyDown_s    =false;
	static bool physicalIsControlKeyDown_s=false;

	// the last known mouse position
	static BPoint physicalPosition_s;
 
 	// the pressed mouse buttons, as received from upstream
 	static int32 physicalButtonsDown_s=0;

	// the position, where the last mouse button went down
 	static BPoint physicalButtonDownPosition_s;

 	// the pressed mouse buttons, as sent to downstream
 	static int32 virtualButtonsDown_s=0;

	// timestamp when the primary, secondare, tertiary physical mouse button was clicked last time
	static int64 virtualButtonDownTime_s[3]={0,0,0};
 
	// the accumulator for the number of clicks of primary, secondary and tertiary
	// mouse buttons, to allow double, triple, ... clicks 
	static int64 virtualClickAccumulator_s[3]={0,0,0};
 
	/* =============================================================
	Variables used for extracting information from the current message
	============================================================= */
 
	// the pressed modifier keys (Shift, Control, ...) that are currently pressed
	int32 physicalModifiers;

	// holds the buttons value of msg
	int32 physicalButtonsDown;
 
	int32 virtualButtonsDown=0;

	/* =============================================================
	Variables required for the filter mechanism
	============================================================= */
	
	// msg holds the message that is going to be injected into outList
	BMessage *msg;

	// the value returned by the filter.
	// This determines the treatment of ther incoming message.
	//   B_DISPATCH_MESSAGE -> incoming mesage is pased on
	//   B_SKIP_MESSAGE     -> incoming message is thrown away
	filter_result filterResult = B_DISPATCH_MESSAGE;

  
	switch (message->what)
	{
		case B_MODIFIERS_CHANGED:
		
			message->FindInt32("modifiers",&physicalModifiers);
		
			physicalIsShiftKeyDown_s  = IS_MODIFIER_SET( B_SHIFT_KEY   );
			physicalIsControlKeyDown_s= IS_MODIFIER_SET( B_CONTROL_KEY );
			physicalIsAltKeyDown_s    = IS_MODIFIER_SET( B_OPTION_KEY  );
		
			if ((physicalIsAltKeyDown_s) && (virtualButtonsDown_s!=0))
			{
				virtualButtonsDown_s=0;
				physicalButtonsDown_s=0;
			
				SEND_MOUSE_UP( virtualButtonsDown_s );
			}
			break;
	
		case B_MOUSE_WHEEL_CHANGED:
			float hfloat;
		  	if ((!physicalIsAltKeyDown_s) && ((physicalIsShiftKeyDown_s) || (configuration.factorsforwheel)))
		  	{                               
				float deltaX, deltaY;
 		  		message->FindFloat("be:wheel_delta_x",&deltaX);
		  		message->FindFloat("be:wheel_delta_y",&deltaY);

				// If the shift key is down, x and y axis get swapped.
				// Thereby mice with a single scroll wheel can be used to scroll
				// in both directions
				if (physicalIsShiftKeyDown_s)
				{
					float tmpFloat;
					tmpFloat = deltaX;
					deltaX   = deltaY;
					deltaY   = tmpFloat;
				}
						
			  	if (configuration.factorsforwheel) 
			  	{
			  		deltaX *= configuration.factorX[ physicalIsControlKeyDown_s ? 1 : 0 ];
			 		deltaY *= configuration.factorY[ physicalIsControlKeyDown_s ? 1 : 0 ];
			 	}

				CREATE_MSG( B_MOUSE_WHEEL_CHANGED );
			  	msg->AddFloat("be:wheel_delta_x",deltaX);
		  		msg->AddFloat("be:wheel_delta_y",deltaY);
		  		ENLIST_MSG();
		  	}
			break;
		
		case B_MOUSE_DOWN:
			if (!physicalIsAltKeyDown_s)
			{
			  	message->FindPoint("where",&physicalButtonDownPosition_s);
			  	message->FindInt32("buttons",&physicalButtonsDown);
				message->FindInt32("modifiers",&physicalModifiers);
		
				// cmdidx is the index of the interclicked command
				int cmdidx = Configuration::getButtonDownIndex( physicalButtonsDown_s, physicalButtonsDown );

				if ( ( cmdidx != -1 ) && ( ! configuration.swallowclick[cmdidx] ) )
				{			
					int new_button_down=0;	
					int new_clicks=0;
				
					switch ( configuration.buttonDownCommand[cmdidx].kind )
					{

						case button:
 						    new_button_down=configuration.buttonDownCommand[cmdidx].mouseButtonIndex;
						    new_clicks=configuration.buttonDownCommand[cmdidx].mouseButtonClicks;
					    
							virtualButtonsDown = virtualButtonsDown_s | ( ButtonDownCommand::mouseButtonIndexToMask( new_button_down ));

							//Reset the accumulator, if last click is too far back
							if ( system_time() - virtualButtonDownTime_s[new_button_down]
							     > configuration.doubleClickSpeed[new_button_down] )
							{
								virtualClickAccumulator_s[new_button_down]=0;
							}
							
							virtualButtonDownTime_s[new_button_down]=system_time();
						
							while (new_clicks>0)
							{
								virtualClickAccumulator_s[new_button_down]++;
								new_clicks--;
								CREATE_MSG( B_MOUSE_DOWN );
								msg->AddPoint("where",physicalButtonDownPosition_s);
								msg->AddInt32("modifiers",physicalModifiers);
								msg->AddInt32("buttons",virtualButtonsDown);
								msg->AddInt32("clicks",virtualClickAccumulator_s[new_button_down]);
								ENLIST_MSG();
						
								if (new_clicks>0)
								{
									SEND_MOUSE_UP( virtualButtonsDown_s );
								}	
							}
					
							virtualButtonsDown_s=virtualButtonsDown;
							break;
											
						case key:
							simulate_keypress( configuration.buttonDownCommand[cmdidx].command, physicalModifiers, outList );
							break;
						
						case cut:
							CREATE_OLD_MSG( 'CCUT' );
							ENLIST_MSG();	
							break;	

						case copy:
							CREATE_OLD_MSG( 'COPY' );
							ENLIST_MSG();	
							break;

						case paste:
							CREATE_OLD_MSG( 'PSTE' );
							ENLIST_MSG();	
							break;
						
						case executable:
							TMsystem( configuration.buttonDownCommand[cmdidx].command );
							filterResult=B_SKIP_MESSAGE;						
							break;		
					}
				 				
				} else {
					filterResult=B_SKIP_MESSAGE;
				}
				physicalButtonsDown_s = physicalButtonsDown;
			}
  			break;	

		case B_MOUSE_UP:
			if (!physicalIsAltKeyDown_s)
			{
			  	message->FindPoint("where",&physicalPosition_s);
			  	message->FindInt32("buttons",&physicalButtonsDown);
				message->FindInt32("modifiers",&physicalModifiers);
	
				filterResult=B_SKIP_MESSAGE;
			
				if (virtualButtonsDown_s!=0)
				{
					if (physicalButtonsDown==0)
					{
						SEND_MOUSE_UP( 0 );
						filterResult=B_DISPATCH_MESSAGE;
						virtualButtonsDown_s=0;
					}
					else
					{
						int cmdidx;
						virtualButtonsDown=0;
					

						REBUILD_VIRTUAL_BUTTONS_DOWN_FROM_PRESSED( B_PRIMARY_MOUSE_BUTTON   );	
						REBUILD_VIRTUAL_BUTTONS_DOWN_FROM_PRESSED( B_SECONDARY_MOUSE_BUTTON );	
						REBUILD_VIRTUAL_BUTTONS_DOWN_FROM_PRESSED( B_TERTIARY_MOUSE_BUTTON  );	
					
						if (virtualButtonsDown!=virtualButtonsDown_s)
						{
							SEND_MOUSE_UP( virtualButtonsDown_s );
							filterResult=B_DISPATCH_MESSAGE;
							virtualButtonsDown_s=virtualButtonsDown;
						}
					}
				}
				physicalButtonsDown_s=physicalButtonsDown;
			}
  		break;	
	
		case B_MOUSE_MOVED:
			if (!physicalIsAltKeyDown_s)
			{
			  	message->FindInt32("buttons",&physicalButtonsDown);
				message->FindInt32("modifiers",&physicalModifiers);
				if (configuration.scrollmousedown[physicalButtonsDown])
				{	
					message->FindPoint("where",&physicalPosition_s);
	
 					float deltaX = configuration.factorX[physicalIsControlKeyDown_s?1:0] * (physicalPosition_s.x-physicalButtonDownPosition_s.x);
					float deltaY = configuration.factorY[physicalIsControlKeyDown_s?1:0] * (physicalPosition_s.y-physicalButtonDownPosition_s.y);
	
					if ((deltaX*deltaX>configuration.minScroll) || (deltaY*deltaY>configuration.minScroll))
					{	
						if (virtualButtonsDown_s!=0)
						{
							SEND_MOUSE_UP( 0 );					
							virtualButtonsDown_s=0;
						
						}
						
						CREATE_MSG( B_MOUSE_WHEEL_CHANGED );
					  	if (physicalIsShiftKeyDown_s) {hfloat=deltaX;deltaX=deltaY;deltaY=hfloat;}
			  			msg->AddFloat("be:wheel_delta_x",deltaX); //so swap x and y here
			  			msg->AddFloat("be:wheel_delta_y",deltaY);
				  		ENLIST_MSG();
					}
		
				}
			}
  		break;	
	
 	}
 	return filterResult;
}
