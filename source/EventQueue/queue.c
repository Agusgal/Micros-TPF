	/***************************************************************************//**
  @file     queue.c
  @brief    eventQueue implementation
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


#define	MAX_NUMBER_OF_EVENTS	15

static Event_Type queue [MAX_NUMBER_OF_EVENTS];
static uint8_t num_Of_Events = 0;
static Event_Type *pin, *pout;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Initializes the circular queue
 */
void queue_Init (void)
{
	pin = queue;
	pout = pin;
	num_Of_Events = 0;
}

/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element(Event_Type event)
{
	// Check for EventQueue Overflow
	if (num_Of_Events > MAX_NUMBER_OF_EVENTS-1)
	{
		return OVERFLOW;
	}

	*pin++ = event;
	num_Of_Events++;

	// Return pointer to the beginning if necessary
	if (pin == MAX_NUMBER_OF_EVENTS + queue)
	{
		pin = queue;
	}

	return num_Of_Events;

}

/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
Event_Type pull_Queue_Element(void)
{
	Event_Type event = *pout;

	if (num_Of_Events == 0)
	{
		return NONE_EV;
	}

	num_Of_Events--;
	pout++;

	if (pout == MAX_NUMBER_OF_EVENTS + queue)
	{
		pout = queue;
	}

	return event;

}


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status()
{
	return num_Of_Events;
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/




/*******************************************************************************
 ******************************************************************************/
