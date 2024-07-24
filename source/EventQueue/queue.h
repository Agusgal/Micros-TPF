/***************************************************************************//**
  @file     queue.h
  @brief    Event queue impelmentation header
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef _queue_H_
#define _queue_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Queue Status
#define	OVERFLOW	-1

// Events
typedef enum {
			NONE_EV,
			START_EV,

			/* BUTTON EVENTS */
			NO_BUTTON_PRESS_EV,

			PLAYPAUSE_EV,
			PLAYPAUSE_LKP_EV,
			STOP_EV,
			STOP_LKP_EV,
			NEXT_EV,
			NEXT_LKP_EV,
			PREV_EV,
			PREV_LKP_EV,


			/* ENCODER EVENTS */
			ENCODER_RIGHT_EV,
			ENCODER_LEFT_EV,
			ENCODER_PRESS_EV,
			ENCODER_LKP_EV,    //Encoder long key press event, todo: puede cambiar.

			TIMEOUT_EV,

			CHANGE_MODE_EV,
		    EFF_SELECTED_EV,

			FILE_SELECTED_EV,
		    SD_IN_EV,
		    SD_OUT_EV,

			NEXT_SONG_EV,
		    PREV_SONG_EV,
		    FILL_BUFFER_EV,


			}Event_Type;


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t pin_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief Initializes the circular queue
 */
void queue_Init (void);


/**
 * @brief Pushes an event to the queue
 * @param event The element to add to the queue
 * @return Number of pending events. Returns value OVERFLOW if the maximun number of events is reached
 */
int8_t push_Queue_Element(Event_Type event);


/**
 * @brief Pulls the earliest event from the queue
 * @return Event_Type variable with the current event if no OVERFLOW is detected.
 */
Event_Type pull_Queue_Element(void);


/**
 * @brief Gets the status of the queue
 * @return Returns the number of pending events in the queue
 */
uint8_t get_Queue_Status();



/*******************************************************************************
 ******************************************************************************/

#endif // _queue_H_
