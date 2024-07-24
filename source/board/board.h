/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
#define PIN_LED_RED     PORTNUM2PIN(PB,22) // PTB22
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26) // PTE26
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21

#define LED_ACTIVE      LOW


// On Board User Switches
#define PIN_SW2         PORTNUM2PIN(PC,6)
#define PIN_SW3         PORTNUM2PIN(PA,4)

#define SW_ACTIVE       LOW
#define SW_INPUT_TYPE   INPUT_PULLDOWN


// Buttons
#define PIN_NEXT_BTN		PORTNUM2PIN(PA,2)
#define PIN_PLAY_BTN		PORTNUM2PIN(PC,2)
#define PIN_PREV_BTN		PORTNUM2PIN(PC,3)
#define PIN_STOP_BTN		PORTNUM2PIN(PC,8)


// Encoder Pins
#define PIN_CH_A			PORTNUM2PIN(PC,0)
#define PIN_CH_B			PORTNUM2PIN(PC,7)
#define PIN_ENC_SW			PORTNUM2PIN(PC,9)

// Test point
#define TP				PORTNUM2PIN(PC,16)
#define TP2				PORTNUM2PIN(PC,17)
/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
