/**********************************************************/
/*                       TMsystem.h                       */
/**********************************************************/
/*                                                        */
/* TMsystem is just like the normal system(...),          */
/* with one exception. This one spawns a seperate thread  */
/* to return to the caller immediately, and still launch  */
/* the desired program                                    */
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
/*  Gisi   2000.08.27    TMsystem (copying command)       */
/*  Gisi   2000.07.30    TMsystem (created)               */
/*                                                        */
/**********************************************************/

#ifndef TM_SYSTEM
#define TM_SYSTEM

int32 TMsystemthreadproc(const char *command)
{
	system(command);
	return B_OK;
}

void TMsystem(const char *incommand)
{
	char command[255];
	strcpy(command,incommand);
	resume_thread(spawn_thread((thread_entry)TMsystemthreadproc, "ownthreadsystem", B_LOW_PRIORITY, (char *)command));
}

#endif
