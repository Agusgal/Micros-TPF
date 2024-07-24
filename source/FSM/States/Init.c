/***************************************************************************/ /**
  @file     init.c
  @brief    Init state functions.
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#include <FSM/States/idle.h>
#include <FSM/States/Init.h>
#include "../../EventQueue/queue.h"
#include "memory_handler.h"
#include "power_mode_switch.h"

void startProgram(void)
{
	//Some SD card shenanigans

	if(mh_SD_connected())
	{
		// Fetch the sd on conection event
		push_Queue_Element(SD_IN_EV);
	}
	else if(mh_is_SD_connected())
	{
		//The sd is present but the event has been fetch while not able to handle
		push_Queue_Element(SD_IN_EV);
	}

	// Start the idle state, not best practice to do here ( ͡° ͜ʖ ͡°)
	Idle_InitState();
}
