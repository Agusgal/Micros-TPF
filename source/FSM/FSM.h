/***************************************************************************//**
  @file     fsm.h
  @brief    finite state machine module, controls program flow
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef _FSM_H_
#define _FSM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "../EventQueue/queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define END_TABLE 0xFF

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


typedef struct state state;

struct state
{
	void(*funct)(void);
	uint8_t event;
	state *next_state;
};


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


extern state INIT_STATE[];
extern state IDLE_STATE[];
extern state FILE_SELECT_STATE[];
extern state AUDIO_PLAYER_STATE[];
extern state EQUALIZER_STATE[];
extern state EQUALIZER_FROM_PLAYER_STATE[];
extern state EQUALIZER_FROM_FILES_STATE[];


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/**
 * @brief determines next state and executes transition functions based on current event, current state and fsm table.
 * @param p_state: pointer to current state inside the fsm state table.
 * @param curr_event: latest event, it came from the event queue.
 * @return pointer to the next state following the logic of the state table.
 */
state* fsm_dispatcher(state* p_state, Event_Type curr_event);


/**
 * @brief returns initial state of the fsm, gets called at the beginning of the program.
 * @return pointer to the initial state of the fsm.
 */
state* get_initial_state(void);



/*******************************************************************************
 ******************************************************************************/

#endif // _FSM_H_
