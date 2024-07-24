/***************************************************************************//**
  @file     Buttons.c
  @brief    Buttons driver.
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>

#include "board.h"
#include "gpio.h"
#include "EventQueue/queue.h"
#include "Buttons.h"
#include "Systick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BUTTON_CALLBACK_PERIOD 50000
#define LKP_COUNTER 5 * S_TO_US / BUTTON_CALLBACK_PERIOD


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

typedef  struct
{
	pin_t pin;
	uint8_t event;
	uint8_t state;
	uint16_t duration_counter;
}Button_t;

Button_t buttons[BUTTON_SIZE];
pin_t  pins []= {PIN_PLAY_BTN, PIN_STOP_BTN, PIN_PREV_BTN, PIN_NEXT_BTN};
enum {PRESSED_B=1, LK_PRESSING_B=2};
enum {IDLE, TAP, LKP};



void Buttons_Update(void);
void ButtonConfig(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Buttons_Init(void)
{
	ButtonConfig();
	SysTick_AddCallback(Buttons_Update, BUTTON_CALLBACK_PERIOD/1000);
}

void Buttons_Update(void)
{
	//gpioWrite(TP, true);

	bool reads[BUTTON_SIZE];
	for (int i=0;i<BUTTON_SIZE;i++)
		reads[i]=gpioRead(buttons[i].pin);

	for (int i=0;i<BUTTON_SIZE;i++)
	{
		if ((buttons[i].state==IDLE) && (reads[i] == false))
		{
			buttons[i].state=PRESSED_B;
			buttons[i].event= IDLE;
			buttons[i].duration_counter++;
		}
		else if ((buttons[i].state == IDLE) && (reads[i] == true)) // && sw_Read == HIGH pero no hace falta
		{
			buttons[i].event = IDLE;
			buttons[i].duration_counter = 0;
		}
		else if ((buttons[i].state == PRESSED_B) && (reads[i] == false))
		{
			buttons[i].duration_counter++;
			if (buttons[i].duration_counter >= LKP_COUNTER)
				buttons[i].state = LK_PRESSING_B;
		}
		else if (buttons[i].state == PRESSED_B)
		{
			buttons[i].state =IDLE;
			buttons[i].event = TAP;
			buttons[i].duration_counter=0;
		}
		else if (buttons[i].state == LK_PRESSING_B && (reads[i] == true))
		{
			buttons[i].state=IDLE;
			buttons[i].event= LKP;
			buttons[i].duration_counter=0;
		}
	}

	//gpioWrite(TP, false);
}

void ButtonConfig(void)
{
	for (int i=0; i < BUTTON_SIZE;i++)
	{
		gpioMode(pins[i],INPUT_PULLUP);
		buttons[i].pin=pins[i];
		buttons[i].state = IDLE;
		buttons[i].event = IDLE,
		buttons[i].duration_counter = 0;
	}
}

Event_Type getButtonEvent(uint8_t button)
{
	if (buttons[button].event != IDLE)
	{
		Event_Type aux = (Event_Type) NO_BUTTON_PRESS_EV + 2 * button + buttons[button].event;
		buttons[button].event = IDLE;
		return aux;
	}
	else
		return NO_BUTTON_PRESS_EV;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
