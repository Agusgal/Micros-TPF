/***************************************************************************//**
  @file     encoder.c
  @brief    Encoder driver
  @author   Grupo 5 - Labo de Micros
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"
#include <stdio.h>
#include "board.h"
#include "Encoder.h"
#include "Systick.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define IDLE 0
#define ENCODER_CALLBACK_PERIOD 10000
#define FIVE_SECOND_COUNTER 1 * S_TO_US / ENCODER_CALLBACK_PERIOD
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int encoder = NO_MOVE;
static int encoder_sw = IDLE_;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void Encoder_Init(void)
{

	gpioMode(PIN_CH_A, INPUT_PULLUP);
	gpioMode(PIN_CH_B, INPUT_PULLUP);
	gpioMode(PIN_ENC_SW, INPUT_PULLUP);
	SysTick_AddCallback(Encoder_Update, 10);
	SysTick_AddCallback(EncoderSwitch_Update, 10);
}

void Encoder_Update(void)
{
	//gpioWrite(TP, true);

	static int state = IDLE;
	bool CH_A = gpioRead(PIN_CH_A);
	bool CH_B = gpioRead(PIN_CH_B);
	encoder= IDLE;

	// State Machine
	switch (state)
	{

		case IDLE:
			if (!CH_A & CH_B)
				state = ACW1;
			else if (CH_A & !CH_B)
				state = CW1;
			else
				state = IDLE;
			break;
		case ACW1:
			if (CH_A & CH_B)
				state = ACW3;
			else if (!CH_A & !CH_B)
				state = ACW2;
			else
				state = ACW1;
			break;
		case ACW2:
			if (!CH_A & CH_B)
				state = ACW1;
			else if (!CH_A & !CH_B)
				state = ACW2;
			else
				state = ACW3;
			break;
		case ACW3:
			if (CH_A & CH_B)
			{
				state = IDLE;
				encoder = LEFT_TURN;
			}
			else if (!CH_A & !CH_B)
				state = ACW2;
			else
				state = ACW3;
			break;
		case CW1:
			if (!CH_A & !CH_B)
				state = CW2;
			else if (CH_A & CH_B)
				state = CW3;
			else
				state = CW1;
			break;
		case CW2:
			if (CH_A & !CH_B)
				state = CW1;
			else if (!CH_A & !CH_B)
				state = CW2;
			else
				state = CW3;
			break;
		case CW3:
			if (CH_A & CH_B)
			{
				state = IDLE;
				encoder = RIGHT_TURN;
			}
			else if (!CH_A & !CH_B)
				state = CW2;
			else
				state = CW3;
			break;
	}

	//gpioWrite(TP, false);
}

void EncoderSwitch_Update(void)
{

	//gpioWrite(TP, true);
	static int sw_state = LOW;
	static int duration_counter=0;
	bool sw_Read=gpioRead(PIN_ENC_SW);
	if ((sw_state==LOW) && (sw_Read == LOW))
	{
		sw_state = HIGH;
		encoder_sw = RISING_FLANK;
		duration_counter++;
	}
	else if (sw_state == LOW)
	{
		encoder_sw = IDLE_;
		duration_counter = 0;
	}
	else if ((sw_state == HIGH) && (sw_Read == LOW))
	{
		duration_counter++;
		if (duration_counter >= FIVE_SECOND_COUNTER)
			encoder_sw = FIVE_SEC_PRESSING;
		else
			encoder_sw = PRESSED;
	}
	else if (sw_state == HIGH)
	{
		sw_state = LOW;
		if (duration_counter >= FIVE_SECOND_COUNTER)
			encoder_sw = FIVE_SEC_PRESS;
		else
			encoder_sw = RELEASED;
		duration_counter = 0;
	}
	//gpioWrite(TP, false);
}

int getEncoderSwitch_State(void)
{
	int aux = encoder_sw;
	encoder_sw = IDLE_;
	return aux;
}

int getEncoder_State(void)
{
	int aux = encoder;
	encoder = NO_MOVE;
	return aux;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
