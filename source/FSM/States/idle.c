/***************************************************************************/ /**
  @file     idle.c
  @brief    Idle state functions.
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <FSM/States/idle.h>
#include <stdio.h>

#include "datetime.h"
#include "../../EventQueue/queue.h"

#include "OLEDdisplay.h"

#include "Systick.h"
#include "Timer.h"


#include "../../drivers/HAl/memory_handler.h"
#include "../mp3_handler/mp3_handler.h"




/*********************************************************
 * 		LOCAL STRUCTS AND ENUMS
 ********************************************************/


/*******************************************************************************
 * PRIVATE VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

int timeCallbackId = -1;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*
 *@brief Callback after changing to high power mode
 */
static void emitStartEv(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void Idle_InitState(void)
{
	//DeInit some modules for Initialization
	mp3Handler_deinit();
}


void Idle_OnUserInteraction(void)
{
	if (!mh_is_SD_connected())
	{
		return;
	}


	if(timeCallbackId != -1)
	{
		Timer_Delete(timeCallbackId);
		timeCallbackId = -1;
	}

	timeCallbackId = Timer_AddCallback(emitStartEv, 3000, true); //Delay until clock stabilizes
}


void UpdateTime()
{
	DateTimeDate_t date = DateTime_GetCurrentDateTime();

	char dateString[16];
	char timeString[16];
	snprintf(dateString, sizeof(dateString), "%02hd-%02hd-%04hd", date.day, date.month, date.year);
	snprintf(timeString, sizeof(timeString), "%02hd:%02hd:%02hd", date.hour,date.minute, date.second);


	OLED_Clear_First_Page();
	OLED_Write_Time(timeString, dateString);
	OLED_Refresh();
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void emitStartEv(void)
{
	timeCallbackId = -1;
	SysTick_UpdateClk();
	push_Queue_Element(START_EV);
}
